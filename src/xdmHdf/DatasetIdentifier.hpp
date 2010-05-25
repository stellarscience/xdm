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
#ifndef xdmHdf_DatasetIdentifier_hpp
#define xdmHdf_DatasetIdentifier_hpp

#include <xdmHdf/ResourceIdentifier.hpp>

#include <xdm/Dataset.hpp>
#include <xdm/RefPtr.hpp>

#include <hdf5.h>

#include <string>



namespace xdmHdf {

class DatasetReleaseFunctor {
public:
  herr_t operator()( hid_t identifier ) {
    return H5Dclose( identifier );
  }
};

typedef ResourceIdentifier< DatasetReleaseFunctor > DatasetIdentifier;

/// Convenience structure for passing dataset creation properties into
/// createDatasetIdentifier.
struct DatasetParameters {
  hid_t parent; ///< Parent identifier.
  std::string name; ///< String name for the dataset.
  int type; ///< Datatype for the dataset.
  hid_t dataspace; ///< HDF5 dataspace identifier.
  xdm::Dataset::InitializeMode mode; ///< Read write or create mode.
  bool chunked; ///< Use chunked IO.
  xdm::DataShape<> chunkSize; ///< the chunk size for chunked IO.
  bool compress; /// Use compression
  int compressionLevel; ///< If using compression, the compression level.
};

/// Create a Dataset identifier with the given parameters.
xdm::RefPtr< DatasetIdentifier > createDatasetIdentifier(
  const DatasetParameters& parameters );

} // namespace xdmHdf

#endif // xdmHdf_DatasetIdentifier_hpp

