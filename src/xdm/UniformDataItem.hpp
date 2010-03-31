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
#ifndef xdm_UniformDataItem_hpp
#define xdm_UniformDataItem_hpp

#include <xdm/Dataset.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/DataItem.hpp>
#include <xdm/MemoryAdapter.hpp>
#include <xdm/PrimitiveType.hpp>
#include <xdm/TypedStructuredArray.hpp>

#include <list>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Representation of a dataset on disk with corresponding shape and type
/// information.  A UniformDataItem contains a data object that
/// should fill the dataspace contained by the UniformDataItem.
class UniformDataItem : public DataItem {
public:

  explicit UniformDataItem(
    primitiveType::Value dataType,
    const DataShape<>& dataspace = DataShape<>() );
  virtual ~UniformDataItem();

  XDM_META_ITEM( UniformDataItem );

  RefPtr< Dataset > dataset();
  RefPtr< const Dataset > dataset() const;
  void setDataset( RefPtr< Dataset > ds );

  /// Set the type for this data.
  void setDataType( primitiveType::Value dataType );
  /// Get the type for this data.
  primitiveType::Value dataType() const;

  /// Get the shape of this data.
  const DataShape<>& dataspace() const;
  /// Set the dataspace for this data item. If any dimension has rank 0, the
  /// internal dataspace ignores that dimension.
  void setDataspace( const DataShape<>& dataspace );

  /// Set the data object that provides memory access to the Item.
  void setData( RefPtr< MemoryAdapter > data );
  /// Get the data object that provides memory access to the Item.
  RefPtr< MemoryAdapter > data();
  /// Get the data object that provides const memory access to the Item.
  RefPtr< const MemoryAdapter > data() const;

  /// Get a typed structured array, if that is how the data is stored
  /// here.
  /// @throws std::runtime_error if the data type is incorrect.
  // TODO: Since the user can already check the data type, maybe this should
  // only throw if the data type cannot be converted to the requested type.
  // In that case, the implementation should try to construct a
  // TypedStructuredArray by copying and converting the data to the
  // requested type.
  template< typename T >
  RefPtr< TypedStructuredArray< T > > typedArray();

  /// Get a typed structured array, if that is how the data is stored
  /// here. Const version.
  /// @throws std::runtime_error if the data type is incorrect.
  template< typename T >
  RefPtr< const TypedStructuredArray< T > > typedArray() const;

  /// Clear all of my writable data.
  void clearData();

  virtual void writeMetadata( XmlMetadataWrapper& xml );

  /// Instruct this data item to initialize it's dataset with the given mode.
  /// @param mode Access mode for the Dataset.
  void initializeDataset( const Dataset::InitializeMode& mode );

  /// Serialize this item's MemoryAdapter to it's dataset.
  /// @pre The item's Dataset has been initialized.
  void serializeData();

  /// Instruct this data item to finalize it's dataset.
  void finalizeDataset();

  /// Determine if this item currently requires serialization . This will return
  /// true if any of the item's MemoryAdapter's is in need of an update.
  bool serializationRequired() const;

private:
  primitiveType::Value mDataType;
  DataShape<> mDataspace;
  RefPtr< Dataset > mDataset;
  RefPtr< MemoryAdapter > mData;
};


// Implementations for the template functions.
#include <xdm/ThrowMacro.hpp>

#include <stdexcept>
#include <string>
#include <typeinfo>

template< typename T >
RefPtr< TypedStructuredArray< T > > UniformDataItem::typedArray() {
  RefPtr< StructuredArray > untyped = data()->array();
  if ( ! untyped ) {
    XDM_THROW( std::logic_error( "typedArray was called on a NULL MemoryAdapter or array." ) );
  }
  RefPtr< TypedStructuredArray< T > > typed =
    dynamic_pointer_cast< TypedStructuredArray< T > >( data()->array() );
  if ( ! typed ) {
    XDM_THROW( std::runtime_error( "In typedArray, the MemoryAdapter does not hold an array"
      " with the requested type: " + std::string( typeid( T ).name() ) ) );
  }
  return typed;
}

template< typename T >
RefPtr< const TypedStructuredArray< T > > UniformDataItem::typedArray() const {
  RefPtr< const StructuredArray > untyped = data()->array();
  if ( ! untyped ) {
    XDM_THROW( std::logic_error( "typedArray was called on a NULL MemoryAdapter or array." ) );
  }
  RefPtr< const TypedStructuredArray< T > > typed =
    dynamic_pointer_cast< const TypedStructuredArray< T > >( data()->array() );
  if ( ! typed ) {
    XDM_THROW( std::runtime_error( "In typedArray, the MemoryAdapter does not hold an array"
      " with the requested type: " + std::string( typeid( T ).name() ) ) );
  }
  return typed;
}

XDM_NAMESPACE_END

#endif // xdm_UniformDataItem_hpp

