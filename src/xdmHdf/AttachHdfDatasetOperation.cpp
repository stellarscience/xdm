//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmHdf/AttachHdfDatasetOperation.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <xdm/Item.hpp>
#include <xdm/UniformDataItem.hpp>

#include <algorithm>
#include <sstream>

XDM_HDF_NAMESPACE_BEGIN

namespace {
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

