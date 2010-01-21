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

#include <stdexcept>

XDM_HDF_NAMESPACE_BEGIN

namespace {

  struct TypeReleaseFunctor {
    htri_t operator()( hid_t typeId ) {
      H5Tclose( typeId );
    }
  };
  typedef ResourceIdentifier< TypeReleaseFunctor > TypeIdentifier;

} // namespace

xdm::RefPtr< DatasetIdentifier > createDatasetIdentifier(
  hid_t parent,
  const std::string& name,
  int type,
  hid_t dataspace,
  const xdm::Dataset::InitializeMode& mode ) {
  
  // check if the dataset already exists within the given parent
  htri_t exists = H5Lexists( parent, name.c_str(), H5P_DEFAULT );
  
  if ( exists ) {

    if ( mode == xdm::Dataset::kCreate ) {

      // the dataset exists and a create was requested, delete the existing one
      H5Ldelete( parent, name.c_str(), H5P_DEFAULT );

    } else {

      // mode is read or modify, make sure the dataset parameters match
      xdm::RefPtr< DatasetIdentifier > datasetId(
        new DatasetIdentifier( H5Dopen( parent, name.c_str(), H5P_DEFAULT ) ) );

      // make sure the space on disk and the requested space match
      xdm::RefPtr< DataspaceIdentifier > datasetSpace(
        new DataspaceIdentifier( H5Dget_space( datasetId->get() ) ) );
      if ( !H5Sextent_equal( datasetSpace->get(), dataspace ) ) {
        XDM_THROW( xdm::DataspaceMismatch( name ) );
      }

      // make sure the type on disk and the requested type match
      xdm::RefPtr< TypeIdentifier > datasetType(
        new TypeIdentifier( H5Dget_type( datasetId->get() ) ) );
      htri_t equalTypes = H5Tequal( type, datasetType->get() );
      if ( equalTypes <= 0 ) {
        XDM_THROW( xdm::DatatypeMismatch( name ) );
      }

      // return the existing dataset
      return datasetId;

    }

  } // if ( exists )

  // the dataset doesn't exist. Read only access is an error.
  if ( mode == xdm::Dataset::kRead ) {
    XDM_THROW( xdm::DatasetNotFound( name ) );
  }

  // the mode is create or modify. In both cases we want to create it if it
  // doesn't yet exist. It is safe to create the dataset here because we deleted
  // the dataset earlier if it existed.
  hid_t datasetId = H5Dcreate(
    parent,
    name.c_str(),
    type,
    dataspace,
    H5P_DEFAULT,
    H5P_DEFAULT,
    H5P_DEFAULT );

  return xdm::RefPtr< DatasetIdentifier >( new DatasetIdentifier( datasetId ) );

}

XDM_HDF_NAMESPACE_END

