
#include <xdmHdf/FileIdentifier.hpp>
#include <xdmHdf/FileIdentifierRegistry.hpp>
#include <xdmHdf/HdfDataset.hpp>
#include <xdmHdf/SelectionVisitor.hpp>

#include <xdm/PrimitiveType.hpp>
#include <xdm/RefPtr.hpp>

#include <algorithm>
#include <map>
#include <sstream>
#include <string>

#include <hdf5.h>

XDM_HDF_NAMESPACE_BEGIN

namespace {

struct HdfTypeMapping {
  std::map< xdm::primitiveType::Value, int > mTypeMap;
  HdfTypeMapping() {
    mTypeMap[xdm::primitiveType::kChar] = H5T_NATIVE_CHAR;
    mTypeMap[xdm::primitiveType::kShort] = H5T_NATIVE_SHORT;
    mTypeMap[xdm::primitiveType::kInt] = H5T_NATIVE_INT;
    mTypeMap[xdm::primitiveType::kLongInt] = H5T_NATIVE_LONG;
    mTypeMap[xdm::primitiveType::kUnsignedChar] = H5T_NATIVE_UCHAR;
    mTypeMap[xdm::primitiveType::kUnsignedShort] = H5T_NATIVE_USHORT;
    mTypeMap[xdm::primitiveType::kUnsignedInt] = H5T_NATIVE_UINT;
    mTypeMap[xdm::primitiveType::kLongUnsignedInt] = H5T_NATIVE_ULLONG;
    mTypeMap[xdm::primitiveType::kFloat] = H5T_NATIVE_FLOAT;
    mTypeMap[xdm::primitiveType::kDouble] = H5T_NATIVE_DOUBLE;
  }
  
  int operator[]( xdm::primitiveType::Value v ) const {
    return (mTypeMap.find( v )->second);
  }
};
static const HdfTypeMapping sHdfTypeMapping;

} // namespace anon

struct HdfDataset::Private {
  std::string mFile;
  std::string mGroup;
  std::string mDataset;
  
  xdm::RefPtr< FileIdentifier > mFileId;
  
  hid_t dataset_identifier;
  hid_t filespace_identifier;
  hid_t group_identifier;

  Private() :
    mFile(),
    mGroup(),
    mDataset(),
    mFileId(),
    dataset_identifier( 0 ),
    filespace_identifier( 0 ),
    group_identifier( 0 ) {}
  Private( 
    const std::string& file,
    const std::string& group,
    const std::string& dataset ) :
    mFile( file ),
    mGroup( group ),
    mDataset( dataset ),
    mFileId(),
    dataset_identifier( 0 ),
    filespace_identifier( 0 ),
    group_identifier( 0 ) {}
};

HdfDataset::HdfDataset() : 
  imp( new Private ){
}

HdfDataset::HdfDataset( 
  const std::string& file,
  const std::string& group,
  const std::string& dataset ) :
  imp( new Private( file, group, dataset ) ) {
}

HdfDataset::~HdfDataset() {
}

void HdfDataset::setFile( const std::string& file ) {
  imp->mFile = file;
}

const std::string& HdfDataset::file() const {
  return imp->mFile;
}

void HdfDataset::setGroup( const std::string& group ) {
  imp->mGroup = group;
}

const std::string& HdfDataset::group() const {
  return imp->mGroup;
}

void HdfDataset::setDataset( const std::string& dataset ) {
  imp->mDataset = dataset;
}

const std::string& HdfDataset::dataset() const {
  return imp->mDataset;
}

void HdfDataset::writeTextContent( xdm::XmlTextContent& text ) {
  std::stringstream out;
  out << imp->mFile << ":" << imp->mGroup << "/" << imp->mDataset;
  text.appendContentLine( out.str() );
}

void HdfDataset::initializeImplementation(
  xdm::primitiveType::Value type,
  const xdm::DataShape<>& shape ) {
  
  // open the HDF file for writing
  imp->mFileId = FileIdentifierRegistry::instance()->findOrCreateIdentifier(
    imp->mFile );
  hid_t datasetLocId = imp->mFileId->get();

  // construct the group in the file.
  if ( !imp->mGroup.empty() ) {
    // first try to open the group to see if it's there
    imp->group_identifier = H5Gopen( 
      imp->mFileId->get(), 
      imp->mGroup.c_str(), 
      H5P_DEFAULT );
    if ( imp->group_identifier < 0 ) {
      imp->group_identifier = H5Gcreate( 
        imp->mFileId->get(), 
        imp->mGroup.c_str(), 
        H5P_DEFAULT,
        H5P_DEFAULT,
        H5P_DEFAULT );
    }
    datasetLocId = imp->group_identifier;
  }
  
  // construct the file space to correspond to the requested shape
  // convert between size type
  xdm::DataShape< hsize_t > file_shape( shape );
  imp->filespace_identifier = H5Screate_simple( file_shape.rank(), &file_shape[0], NULL );

  // construct the dataset in the file
  imp->dataset_identifier = H5Dcreate(
    datasetLocId,
    imp->mDataset.c_str(),
    sHdfTypeMapping[type],
    imp->filespace_identifier,
    H5P_DEFAULT,
    H5P_DEFAULT,
    H5P_DEFAULT );
}

void HdfDataset::serializeImplementation(
  const xdm::StructuredArray* data ) {

  // create the memory space to match the shape of the array
  // convert between types for size representation
  xdm::DataShape< hsize_t > memory_shape( data->shape() );
  hid_t memory_space = H5Screate_simple( 
    memory_shape.rank(),
    &memory_shape[0],
    NULL );

  SelectionVisitor memspaceSelector( memory_space );
  data->selection()->accept( memspaceSelector );
  SelectionVisitor filespaceSelector( imp->filespace_identifier );
  selection()->accept( filespaceSelector );

  // write the array to disk
  H5Dwrite( 
    imp->dataset_identifier, 
    sHdfTypeMapping[data->dataType()], 
    memory_space, 
    imp->filespace_identifier,
    H5P_DEFAULT,
    data->data() );
}

void HdfDataset::finalizeImplementation() {
  H5Sclose( imp->filespace_identifier );
  H5Dclose( imp->dataset_identifier );
  if ( imp->group_identifier ) {
    H5Gclose( imp->group_identifier );
  }
}

XDM_HDF_NAMESPACE_END

