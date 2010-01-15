//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.      
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
#ifndef xdm_UniformDataItem_hpp
#define xdm_UniformDataItem_hpp

#include <xdm/Dataset.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/DataItem.hpp>
#include <xdm/PrimitiveType.hpp>
#include <xdm/WritableData.hpp>

#include <list>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Representation of a dataset on disk with corresponding shape and type
/// information.  A UniformDataItem contains one or more data objects that
/// should fill the dataspace contained by the UniformDataItem.
class UniformDataItem : public DataItem {
public:
  explicit UniformDataItem( 
    primitiveType::Value dataType,
    const DataShape<>& dataspace = DataShape<>() );
  virtual ~UniformDataItem();

  XDM_META_ITEM( UniformDataItem );

  Dataset* dataset();
  const Dataset* dataset() const;
  void setDataset( Dataset* ds );

  /// Set the type for this data.
  void setDataType( primitiveType::Value dataType );
  /// Get the type for this data.
  primitiveType::Value dataType() const;

  /// Get the shape of this data. 
  const DataShape<>& dataspace() const;
  /// Set the dataspace for this data item.
  void setDataspace( const DataShape<>& dataspace );

  /// Append a data object to the list of data objects that will make up my
  /// space.
  void appendData( WritableData* data );
  /// Clear all of my writable data.
  void clearData();

  virtual void writeMetadata( XmlMetadataWrapper& xml );

  /// Virtual function to serialize the data contained in the item.  Inheritors
  /// should reimplement this function to specify what data should be written to
  /// a dataset.
  void serializeData();

private:
  primitiveType::Value mDataType;
  DataShape<> mDataspace;
  RefPtr< Dataset > mDataset;
  std::list< RefPtr< WritableData > > mWritables;
};

XDM_NAMESPACE_END

#endif // xdm_UniformDataItem_hpp

