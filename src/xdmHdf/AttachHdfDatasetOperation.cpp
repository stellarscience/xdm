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

class GroupDatasetForSeriesIndex :
  public xdm::DatasetUpdateCallback< HdfDataset > {
  virtual void update( HdfDataset * dataset, std::size_t seriesIndex) {
    GroupPath gpath = dataset->groupPath();
    std::ostringstream ss;
    ss << seriesIndex;
    gpath[0] = ss.str();
    dataset->setGroupPath( gpath );
  }
};

void spacesToUnderscores( std::string& s ) {
  using std::string;
  static const char * kSpace = " ";
  std::size_t current = 0;
  while( ( current = s.find_first_of( kSpace, current ) ) != string::npos ) {
    s[current] = '_';
  }
}
    

} // namespace anon

AttachHdfDatasetOperation::AttachHdfDatasetOperation( 
  const std::string& fileName,
  bool groupBySeriesIndex ) :
  mFileName( fileName ),
  mGroupBySeriesIndex( groupBySeriesIndex ),
  mCurrentPath() {
  if ( mGroupBySeriesIndex ) {
    // Put a top level name in for data that is valid for all steps. If grouping
    // by series index has been requested, this name will be replaced by the
    // step index later. 
    mCurrentPath.push_back( "All" );
  }
}

AttachHdfDatasetOperation::~AttachHdfDatasetOperation() {
}

void AttachHdfDatasetOperation::apply( xdm::Item& item ) {
  std::string name = item.name();
  if ( name.empty() ) {
    name = item.className();
  }

  spacesToUnderscores( name );

  mCurrentPath.push_back( name );

  traverse( item );

  mCurrentPath.pop_back();
}

void AttachHdfDatasetOperation::apply( xdm::UniformDataItem& item ) {
  // If the item already has a dataset, we do nothing.
  if ( item.dataset() ) {
    return;
  }

  // If the data item has a name use it, otherwise generate a unique name with
  // the string value of the pointer to the data item.
  std::string name;
  if ( !item.name().empty() ) {
    name = item.name();
  } else {
    std::ostringstream ss;
    ss << &item;
    name = ss.str();
  }

  spacesToUnderscores( name );

  // The item has no dataset, build an HDF dataset for the current group path.
  xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset );
  dataset->setFile( mFileName ),
  dataset->setGroupPath( mCurrentPath );
  dataset->setDataset( name );
  item.setDataset( dataset );

  // If requested, group the datasets by series index.
  if ( mGroupBySeriesIndex && item.data()->isDynamic() ) {
    dataset->setUpdateCallback( xdm::makeRefPtr( new GroupDatasetForSeriesIndex ) );
  }
}

XDM_HDF_NAMESPACE_END

