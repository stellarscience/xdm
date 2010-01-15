
#include <xdm/HdfDataset.hpp>

#include <algorithm>
#include <sstream>
#include <string>

#include <hdf5.h>

namespace {

struct HdfLocation {  
  std::string mFilePath;
  std::string mDatasetPath;
  HdfLocation( const std::string& data ) {
    size_t pos = data.find( ':' );
    mFilePath = data.substr(0, pos );
    mDatasetPath = data.substr( pos + 1 );
  }
};

} // namespace anon

XDM_NAMESPACE_BEGIN

struct HdfDataset::Private {
  hid_t file_identifier;
  hid_t dataset_identifier;
  hid_t filespace_identifier;
};

HdfDataset::HdfDataset() : 
  imp( new Private ){
}

HdfDataset::~HdfDataset() {
}

void HdfDataset::initializeImplementation( 
  const DataShape<>& shape, 
  std::iostream& content ) {
  
  // in an HDF dataset, the initialization content looks like:
  // <filename>.h5:/path/to/dataset
  std::stringstream init;
  init << content.rdbuf();
  HdfLocation locator( init.str() );

  // open the HDF file for writing
  imp->file_identifier = H5Fcreate( 
    locator.mFilePath.c_str(),
    H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );

  // construct the file space to correspond to the requested shape
  // convert between size type
  DataShape< hsize_t > file_shape( shape );
  imp->filespace_identifier = H5Screate_simple( file_shape.rank(), &file_shape[0], NULL );

  // construct the dataset in the file
  imp->dataset_identifier = H5Dcreate(
    imp->file_identifier,
    locator.mDatasetPath.c_str(),
    H5T_NATIVE_FLOAT,
    imp->filespace_identifier,
    H5P_DEFAULT,
    H5P_DEFAULT,
    H5P_DEFAULT );
}

void HdfDataset::serializeImplementation(
  const Array& data,
  const HyperSlabMap<>& memory_map,
  std::iostream& content ) {

  // create the memory space to match the shape of the array
  // convert between types for size representation
  DataShape< hsize_t > memory_shape( data.shape() );
  hid_t memory_space = H5Screate_simple( 
    memory_shape.rank(),
    &memory_shape[0],
    NULL );

  // select the hyperslab of the data in memory
  HyperSlab< hsize_t > domain( memory_map.domain() );
  H5Sselect_hyperslab( 
    memory_space,
    H5S_SELECT_SET, 
    &(domain.start( 0 )), 
    &(domain.stride( 0 )),
    &(domain.count( 0 )),
    NULL );

  // select the hyperslab of the data on disk
  HyperSlab< hsize_t > range( memory_map.range() );
  H5Sselect_hyperslab(
    imp->filespace_identifier,
    H5S_SELECT_SET,
    &(range.start(0)),
    &(range.stride(0)),
    &(range.count(0)),
    NULL );

  // write the array to disk
  H5Dwrite( 
    imp->dataset_identifier, 
    H5T_NATIVE_FLOAT, 
    memory_space, 
    imp->filespace_identifier,
    H5P_DEFAULT,
    data.data() );
}

void HdfDataset::finalizeImplementation() {
  H5Sclose( imp->filespace_identifier );
  H5Dclose( imp->dataset_identifier );
  H5Fclose( imp->file_identifier );
}

XDM_NAMESPACE_END

