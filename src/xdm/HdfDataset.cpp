
#include <xdm/HdfDataset.hpp>
#include <xdm/PrimitiveType.hpp>

#include <algorithm>
#include <map>
#include <sstream>
#include <string>

#include <hdf5.h>

XDM_NAMESPACE_BEGIN

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

struct HdfTypeMapping {
  std::map< primitiveType::Value, int > mTypeMap;
  HdfTypeMapping() {
    mTypeMap[primitiveType::kChar] = H5T_NATIVE_CHAR;
    mTypeMap[primitiveType::kShort] = H5T_NATIVE_SHORT;
    mTypeMap[primitiveType::kInt] = H5T_NATIVE_INT;
    mTypeMap[primitiveType::kLongInt] = H5T_NATIVE_LONG;
    mTypeMap[primitiveType::kUnsignedChar] = H5T_NATIVE_UCHAR;
    mTypeMap[primitiveType::kUnsignedShort] = H5T_NATIVE_USHORT;
    mTypeMap[primitiveType::kUnsignedInt] = H5T_NATIVE_UINT;
    mTypeMap[primitiveType::kLongUnsignedInt] = H5T_NATIVE_ULLONG;
    mTypeMap[primitiveType::kFloat] = H5T_NATIVE_FLOAT;
    mTypeMap[primitiveType::kDouble] = H5T_NATIVE_DOUBLE;
  }
  
  int operator[]( primitiveType::Value v ) const {
    return (mTypeMap.find( v )->second);
  }
};
static const HdfTypeMapping sHdfTypeMapping;

} // namespace anon

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
  primitiveType::Value type,
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
    sHdfTypeMapping[type],
    imp->filespace_identifier,
    H5P_DEFAULT,
    H5P_DEFAULT,
    H5P_DEFAULT );
}

void HdfDataset::serializeImplementation(
  const StructuredArray& data,
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
    sHdfTypeMapping[data.dataType()], 
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

