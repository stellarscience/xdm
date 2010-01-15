#include <xdmHdf/AttachHdfDatasetOperation.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <xdm/Item.hpp>
#include <xdm/UniformDataItem.hpp>

#include <algorithm>
#include <sstream>

XDM_HDF_NAMESPACE_BEGIN

namespace {
  struct AppendGroup {
    std::stringstream& mStream;
    AppendGroup( std::stringstream& stream ) : mStream( stream ) {}
    void operator()( const std::string& groupName ) {
      mStream << "/" << groupName;
    }
  };

  std::string makePath( const std::vector< std::string >& groups ) {
    if ( groups.empty() ) {
      return std::string();
    }

    std::vector< std::string >::const_iterator start = groups.begin();
    std::stringstream groupString;
    groupString << "/" << *start++;
    std::for_each( start, groups.end(), AppendGroup( groupString ) );
    return groupString.str();
  }
} // namespace anon

AttachHdfDatasetOperation::AttachHdfDatasetOperation( 
  const std::string& fileName,
  const std::string& commonName ) :
  mFileName( fileName ),
  mCommonName( commonName ),
  mCurrentPath() {
}

AttachHdfDatasetOperation::~AttachHdfDatasetOperation() {
}

void AttachHdfDatasetOperation::apply( xdm::Item& item ) {
  const std::string& name = item.name();
  if ( name.empty() ) {
    mCurrentPath.push_back( item.className() );
  } else {
    mCurrentPath.push_back( name );
  }

  traverse( item );

  mCurrentPath.pop_back();
}

void AttachHdfDatasetOperation::apply( xdm::UniformDataItem& item ) {
  // If the item already has a dataset, we do nothing.
  if ( item.dataset() ) {
    return;
  }

  // The item has no dataset, build an HDF dataset for the current group path.
  xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset );
  dataset->setFile( mFileName ),
  dataset->setGroupPath( mCurrentPath );
  dataset->setDataset( mCommonName );
  item.setDataset( dataset );
}

XDM_HDF_NAMESPACE_END

