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

  // Try to choose a good name for the dataset.
  std::string name = mCommonName;
  if ( !item.name().empty() ) {
    name = item.name();
  }

  // The item has no dataset, build an HDF dataset for the current group path.
  xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset );
  dataset->setFile( mFileName ),
  dataset->setGroupPath( mCurrentPath );
  dataset->setDataset( name );
  item.setDataset( dataset );
}

XDM_HDF_NAMESPACE_END

