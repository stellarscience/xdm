//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmHdf/DatasetIdentifier.hpp>
#include <xdmHdf/DataspaceIdentifier.hpp>
#include <xdmHdf/FileIdentifier.hpp>
#include <xdmHdf/FileIdentifierRegistry.hpp>
#include <xdmHdf/GroupIdentifier.hpp>
#include <xdmHdf/HdfDataset.hpp>
#include <xdmHdf/SelectionVisitor.hpp>

#include <xdm/PrimitiveType.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/ThrowMacro.hpp>

#include <algorithm>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include <hdf5.h>

XDM_HDF_NAMESPACE_BEGIN

namespace {

// By default, the HDF library will clean up all identifiers upon application
// exit.  The following class contains instructions that tell HDF not to do
// this.  This is necessary because we hold on to certain identifiers in a
// singleton registry object, whose at exit destructor takes care of the
// identifiers.  This means uncareful programming can cause HDF to leak, so it
// is important to manage HDF resources.  An RAII class, ResourceIdentifier is
// available to assist with this.
class HdfInitializationInstruction {
public:
  HdfInitializationInstruction() {
    H5dont_atexit();
  }
};
static HdfInitializationInstruction initHdf;

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

struct AppendGroup {
  std::stringstream& mStream;
  AppendGroup( std::stringstream& stream ) : mStream( stream ) {}
  void operator()( const std::string& groupName ) {
    mStream << "/" << groupName;
  }
};

} // namespace anon

struct HdfDataset::Private {
  std::string mFile;
  GroupPath mGroupPath;
  std::string mDataset;
  
  xdm::RefPtr< FileIdentifier > mFileId;
  xdm::RefPtr< GroupIdentifier > mGroupId;
  xdm::RefPtr< DatasetIdentifier > mDatasetId;
  xdm::RefPtr< DataspaceIdentifier > mDataspaceId; 

  Private() :
    mFile(),
    mGroupPath(),
    mDataset(),
    mFileId(),
    mGroupId(),
    mDatasetId(),
    mDataspaceId() {}
  Private( 
    const std::string& file,
    const GroupPath& groupPath,
    const std::string& dataset ) :
    mFile( file ),
    mGroupPath( groupPath ),
    mDataset( dataset ),
    mFileId(),
    mGroupId(),
    mDatasetId(),
    mDataspaceId() {}
};

HdfDataset::HdfDataset() : 
  imp( new Private ){
}

HdfDataset::HdfDataset( 
  const std::string& file,
  const GroupPath& groupPath,
  const std::string& dataset ) :
  imp( new Private( file, groupPath, dataset ) ) {
}

HdfDataset::~HdfDataset() {
}

void HdfDataset::setFile( const std::string& file ) {
  imp->mFile = file;
}

const std::string& HdfDataset::file() const {
  return imp->mFile;
}

void HdfDataset::setGroupPath( const GroupPath& groupPath ) {
  imp->mGroupPath = groupPath;
}

const GroupPath& HdfDataset::groupPath() const {
  return imp->mGroupPath;
}

void HdfDataset::setDataset( const std::string& dataset ) {
  imp->mDataset = dataset;
}

const std::string& HdfDataset::dataset() const {
  return imp->mDataset;
}

void HdfDataset::writeTextContent( xdm::XmlTextContent& text ) {
  std::stringstream out;
  out << imp->mFile << ":";
  std::for_each( imp->mGroupPath.begin(), imp->mGroupPath.end(), 
    AppendGroup( out ) );
  out << "/" << imp->mDataset;
  text.appendContentLine( out.str() );
}

void HdfDataset::initializeImplementation(
  xdm::primitiveType::Value type,
  const xdm::DataShape<>& shape ) {
  
  // open the HDF file for writing
  imp->mFileId = createFileIdentifier( imp->mFile );
  hid_t datasetLocId = imp->mFileId->get();

  // construct the group in the file.
  if ( !imp->mGroupPath.empty() ) {
    hid_t parentIdentifier = imp->mFileId->get();
    for ( 
      GroupPath::iterator group = imp->mGroupPath.begin();
      group != imp->mGroupPath.end();
      ++group ) {
      // Note: we are using the raw resource to identify the parent.  This is ok
      // because the parent resource will be released after the call to 
      // createGroupIdentifier.
      imp->mGroupId = createGroupIdentifier( parentIdentifier, *group );
      parentIdentifier = imp->mGroupId->get();
    }  
    datasetLocId = imp->mGroupId->get();
  }
  
  // construct the file space to correspond to the requested shape
  // convert between size type
  imp->mDataspaceId = createDataspaceIdentifier( shape );

  // construct the dataset in the file
  imp->mDatasetId = createDatasetIdentifier(
    datasetLocId,
    imp->mDataset,
    sHdfTypeMapping[type],
    imp->mDataspaceId->get() );
}

void HdfDataset::serializeImplementation(
  const xdm::StructuredArray* data,
  const xdm::DataSelectionMap& selectionMap ) {

  // create the memory space to match the shape of the array
  // convert between types for size representation
  xdm::RefPtr< DataspaceIdentifier > memorySpace =
    createDataspaceIdentifier( xdm::makeShape( data->size() ) );

  SelectionVisitor memspaceSelector( memorySpace->get() );
  selectionMap.domain()->accept( memspaceSelector );
  SelectionVisitor filespaceSelector( imp->mDataspaceId->get() );
  selectionMap.range()->accept( filespaceSelector );

  hssize_t datasetNumpoints = H5Sget_select_npoints( imp->mDataspaceId->get() );
  hssize_t arrayNumpoints = H5Sget_select_npoints( memorySpace->get() );
  if ( datasetNumpoints != arrayNumpoints ) {
    std::stringstream ss;
    ss << "Data size mismatch: ";
    ss << "dataset, " << datasetNumpoints;
    ss << " array, " << arrayNumpoints;
    XDM_THROW( std::runtime_error( ss.str() ) );
  }

  // write the array to disk
  H5Dwrite( 
    imp->mDatasetId->get(), 
    sHdfTypeMapping[data->dataType()], 
    memorySpace->get(), 
    imp->mDataspaceId->get(),
    H5P_DEFAULT,
    data->data() );
}

void HdfDataset::finalizeImplementation() {
  H5Fflush( imp->mFileId->get(), H5F_SCOPE_GLOBAL );
}

XDM_HDF_NAMESPACE_END

