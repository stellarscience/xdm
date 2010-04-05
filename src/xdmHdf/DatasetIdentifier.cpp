//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.      
//                                                                             
// This file is part of XDM                                                    
//                                                                             
// This program is free software: you can redistribute it and/or modify it     
// under the terms of the GNU Lesser General Public License as published by    
// the Free Software Foundation, either version 3 of the License, or (at your  
// option) any later version.                                                  
//                                                                             
// This program is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        
// License for more details.                                                   
//                                                                             
// You should have received a copy of the GNU Lesser General Public License    
// along with this program.  If not, see <http://www.gnu.org/licenses/>.       
//                                                                             
//------------------------------------------------------------------------------
#include <xdmHdf/DatasetIdentifier.hpp>
#include <xdmHdf/DataspaceIdentifier.hpp>

#include <xdm/DatasetExcept.hpp>
#include <xdm/ThrowMacro.hpp>

#include <hdf5.h>

#include <algorithm>
#include <functional>
#include <stdexcept>

XDM_HDF_NAMESPACE_BEGIN

namespace {

// RAII object to manage HDF5 type identifiers.
struct TypeReleaseFunctor {
  htri_t operator()( hid_t typeId ) {
    H5Tclose( typeId );
  }
};
typedef ResourceIdentifier< TypeReleaseFunctor > TypeIdentifier;

// RAII object to manage HDF5 property list identifiers.
struct PListReleaseFunctor {
  htri_t operator()( hid_t propertyId ) {
    if ( propertyId != H5P_DEFAULT ) {
      H5Pclose( propertyId );
    }
  }
};
typedef ResourceIdentifier< PListReleaseFunctor > PListIdentifier;

xdm::DataShape<> h5sToShape( hid_t space ) {
  int rank = H5Sget_simple_extent_ndims( space );
  xdm::DataShape< hsize_t > retvalue( rank );
  H5Sget_simple_extent_dims( space, &retvalue[0], NULL );
  return retvalue;
}

// Open an existing dataset for read or modify access.
xdm::RefPtr< DatasetIdentifier > openExistingDataset(
  const DatasetParameters& parameters )
{
  hid_t datasetHid = H5Dopen(
    parameters.parent,
    parameters.name.c_str(),
    H5P_DEFAULT );
  xdm::RefPtr< DatasetIdentifier > datasetId;
  if ( datasetHid < 0 ) {
    XDM_THROW( xdm::DatasetNotFound( parameters.name ) );
  } else {
    datasetId = new DatasetIdentifier( datasetHid );
  }

  // make sure the space on disk and the requested space match
  xdm::RefPtr< DataspaceIdentifier > datasetSpace(
      new DataspaceIdentifier( H5Dget_space( datasetId->get() ) ) );

  H5S_class_t datasetSpaceClass = H5Sget_simple_extent_type( datasetSpace->get() );
  H5S_class_t parameterSpaceClass = H5Sget_simple_extent_type( parameters.dataspace );
  if ( datasetSpaceClass != parameterSpaceClass ) {
    XDM_THROW( xdm::DatasetError( parameters.name, "HDF5 dataspace classes don't match." ) );
  }
  htri_t equalExtents = H5Sextent_equal( datasetSpace->get(), parameters.dataspace );
  if ( equalExtents <= 0 ) {
    xdm::DataShape<> datasetShape = h5sToShape( datasetSpace->get() );
    xdm::DataShape<> parameterShape = h5sToShape( parameters.dataspace );
    XDM_THROW( xdm::DataspaceMismatch(
      parameters.name,
      datasetShape,
      parameterShape ) );
  }

  // make sure the type on disk and the requested type match
  xdm::RefPtr< TypeIdentifier > datasetType(
      new TypeIdentifier( H5Dget_type( datasetId->get() ) ) );
  htri_t equalTypes = H5Tequal( parameters.type, datasetType->get() );
  if ( equalTypes <= 0 ) {
    XDM_THROW( xdm::DatatypeMismatch( parameters.name ) );
  }

  // return the existing dataset
  return datasetId;
}

// Set up a PList identifier for chunking with the given size and space.
void setupChunks( hid_t plist, const xdm::DataShape<>& chunkShape, hid_t spaceId ) {
  xdm::DataShape<> spaceShape( h5sToShape( spaceId ) );

  // Make sure the rank of the chunks equals the rank of the space
  if ( spaceShape.rank() != chunkShape.rank() ) {
    XDM_THROW( xdm::DataspaceMismatch(
      "Chunked Dataset",
      chunkShape,
      spaceShape ) );
  }

  xdm::DataShape< hsize_t > resultShape( chunkShape.rank() );

  // Ensure that the chunk size in any dimension does not exceed the
  // corresponding dimension in the dataset.
  xdm::DataShape< hsize_t >::DimensionIterator result = resultShape.begin();
  for ( xdm::DataShape<>::ConstDimensionIterator
    chunk = chunkShape.begin(), space = spaceShape.begin();
    chunk != chunkShape.end();
    ++chunk, ++space )
  {
    *result++ = std::min( *chunk, *space );
  }

  // Now we are guaranteed that each dimension of the chunk size does not
  // exceed the corresponding dataset dimension.
  H5Pset_chunk(
    plist,
    resultShape.rank(),
    &(resultShape[0]) );
}

// Set up a PList identifier for compression with the given level.
void setupCompression( hid_t plist, size_t level ) {
  // make sure compression is available.
  if ( H5Zfilter_avail( H5Z_FILTER_DEFLATE ) ) {
    H5Pset_deflate( plist, level );
  }
}

} // namespace

//------------------------------------------------------------------------------
xdm::RefPtr< DatasetIdentifier > createDatasetIdentifier(
  const DatasetParameters& parameters ) {
  
  // check if the dataset already exists within the given parent
  htri_t exists = H5Lexists(
    parameters.parent,
    parameters.name.c_str(),
    H5P_DEFAULT );
  
  if ( exists ) {
    if ( parameters.mode == xdm::Dataset::kCreate ) {
      // the dataset exists and a create was requested, delete the existing one
      H5Ldelete( parameters.parent, parameters.name.c_str(), H5P_DEFAULT );
    } else {
      // read or modify access, open and return the existing dataset
      return openExistingDataset( parameters );
    }
  }

  // the dataset doesn't exist. Read only access is an error.
  if ( parameters.mode == xdm::Dataset::kRead ) {
    XDM_THROW( xdm::DatasetNotFound( parameters.name ) );
  }

  // Determine the dataset access properties based off chunking and compression
  // parameters.
  xdm::RefPtr< PListIdentifier > createPList( new PListIdentifier( H5P_DEFAULT ) );
  if ( parameters.chunked ) {
    createPList->reset( H5Pcreate( H5P_DATASET_CREATE ) );
    setupChunks( createPList->get(), parameters.chunkSize, parameters.dataspace );
    // Chunking is enabled, so check for compression and enable it if possible.
    if ( parameters.compress ) {
      setupCompression( createPList->get(), parameters.compressionLevel );
    }
  }

  // the mode is create or modify. In both cases we want to create it if it
  // doesn't yet exist. It is safe to create the dataset here because we deleted
  // the dataset earlier if it existed.
  hid_t datasetId = H5Dcreate(
    parameters.parent,
    parameters.name.c_str(),
    parameters.type,
    parameters.dataspace,
    H5P_DEFAULT,
    createPList->get(),
    H5P_DEFAULT );

  return xdm::RefPtr< DatasetIdentifier >( new DatasetIdentifier( datasetId ) );

}

XDM_HDF_NAMESPACE_END

