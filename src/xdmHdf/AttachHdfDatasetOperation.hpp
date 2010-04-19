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
#ifndef xdmHdf_AttachHdfDatasetOperation_hpp
#define xdmHdf_AttachHdfDatasetOperation_hpp

#include <xdmHdf/HdfDataset.hpp>

#include <xdm/ItemVisitor.hpp>

#include <string>
#include <vector>

#include <xdmHdf/NamespaceMacro.hpp>

XDM_HDF_NAMESPACE_BEGIN

/// Operation on the XDM tree to attach an HDF dataset to all UniformDataItems.
/// Applying this visitor to an XDM tree will force all UniformDataItems to
/// write their arrays to an HDF database.  The path to the database in the HDF
/// file will be determined by the Item name, or failing that, the class name
/// for the parents of the UniformDataItem. The Dataset will take either the
/// name of the UniformDataItem it belongs to (if it has one), or a unique
/// identifier generated automatically based on the UniformDataItem it belongs
/// to.
///
/// In addition, this visitor takes a parameter that allows HDF datasets to
/// be grouped by series index, allowing new data to be written as a series
/// progresses.
///
/// If the UniformDataItem already has a dataset attached, no action will be
/// taken.
class AttachHdfDatasetOperation : public xdm::ItemVisitor {
public:
  /// Takes a file name and a common name for all datasets.
  AttachHdfDatasetOperation( 
    const std::string& fileName,
    bool groupBySeriesIndex );
  virtual ~AttachHdfDatasetOperation();

  //-- ItemVisitor implementations --//
  virtual void apply( xdm::Item& item );
  virtual void apply( xdm::UniformDataItem& item );

private:
  std::string mFileName;
  bool mGroupBySeriesIndex;
  GroupPath mCurrentPath;
};

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_AttachHdfDatasetOperation_hpp
