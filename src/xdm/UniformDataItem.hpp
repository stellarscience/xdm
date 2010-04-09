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

#include <cassert>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Base class for errors signalling problems when accessing data in memory or
/// on disk.
class DataAccessError : public std::runtime_error {
public:
  DataAccessError() : std::runtime_error( "Item could not access data." ) {}
  virtual ~DataAccessError() throw() {}
};

/// Representation of a dataset on disk with corresponding shape and type
/// information.  A UniformDataItem contains a data object that
/// should fill the dataspace contained by the UniformDataItem.
class UniformDataItem : public DataItem {
public:

  /// Constructor initializes with a 4 byte floating point type specification.
  /// and an empty dataspace.
  UniformDataItem();

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

  /// Get a value by reference indexed contiguously in the underlying data.
  template< typename T >
  T& atIndex( std::size_t index );
  /// Get a value by const reference indexed contiguously in the underlying data.
  template< typename T >
  const T& atIndex( std::size_t index ) const;

  /// Get a value by reference at a location specified by a DataShape.
  template< typename T >
  T& atLocation( const xdm::DataShape<>& location );
  /// Get a value by const reference at a location specified by a DataShape.
  template< typename T >
  const T& atLocation( const xdm::DataShape<>& location ) const;

  /// @defgroup Convenience functions for common data ranks.
  /// @{
  template< typename T >
  T& atLocation( std::size_t i );
  template< typename T >
  const T& atLocation( std::size_t i ) const;

  template< typename T >
  T& atLocation( std::size_t i, std::size_t j );
  template< typename T >
  const T& atLocation( std::size_t i, std::size_t j ) const;

  template< typename T >
  T& atLocation( std::size_t i, std::size_t j, std::size_t k );
  template< typename T >
  const T& atLocation( std::size_t i, std::size_t j, std::size_t k ) const;

  template< typename T >
  T& atLocation( std::size_t i, std::size_t j, std::size_t k, std::size_t l );
  template< typename T >
  const T& atLocation( std::size_t i, std::size_t j, std::size_t k, std::size_t l ) const;
  /// @}

  /// Clear all of my writable data.
  void clearData();

  virtual void writeMetadata( XmlMetadataWrapper& xml );

  /// Instruct this data item to initialize it's dataset with the given mode.
  /// @param mode Access mode for the Dataset.
  void initializeDataset( const Dataset::InitializeMode& mode );

  /// Serialize this item's MemoryAdapter to it's dataset.
  /// @pre The item's Dataset has been initialized.
  void serializeData();

  /// Deserialize data from the dataset to this item's MemoryAdapter.
  /// @pre The item's Dataset has been initialized.
  /// @post The item's MemoryAdapter contains the data from the dataset.
  void deserializeData();

  /// Instruct this data item to finalize it's dataset.
  void finalizeDataset();

  /// Determine if this item currently requires serialization . This will return
  /// true if any of the item's MemoryAdapter's is in need of an update.
  bool serializationRequired() const;

protected:

  /// Get the underlying data array as an untyped StructuredArray. Calls to this
  /// when the underlying data requires an update will case data to be read from
  /// disk. This version returns an immutable array.
  /// @throw DataAccessError The item's data cannot be found.
  virtual RefPtr< const StructuredArray > array() const;

  /// Get the underlying data array as an untyped StructuredArray. Calls to this
  /// when the underlying data requires an update will case data to be read from
  /// disk. This version returns a mutable array.
  /// @throw DataAccessError The item's data cannot be found.
  virtual RefPtr< StructuredArray > array();

private:
  // check the shape bounds against this object's dataspace.
  bool validateBounds( const xdm::DataShape<>& shape ) const;
  primitiveType::Value mDataType;
  DataShape<> mDataspace;
  RefPtr< Dataset > mDataset;
  RefPtr< MemoryAdapter > mData;
};

// -----------------------------------------------------------------------------
// Implementations for the template functions.
// -----------------------------------------------------------------------------
#include <xdm/ThrowMacro.hpp>

#include <stdexcept>
#include <string>
#include <typeinfo>

#include <cassert>

template< typename T >
RefPtr< TypedStructuredArray< T > > UniformDataItem::typedArray() {
  return const_pointer_cast< TypedStructuredArray< T > >(
    static_cast< const UniformDataItem& >(*this).typedArray< T >()
  );
}

template< typename T >
RefPtr< const TypedStructuredArray< T > > UniformDataItem::typedArray() const {
  RefPtr< const StructuredArray > untyped = array();
  if ( ! untyped ) {
    XDM_THROW( DataAccessError() );
  }
  // Performance optimization: Clients are aware of the data type they should be
  // accessing. Rather than a general dynamic cast below, we assert in debug
  // the type and perform a static cast. This provides an error when
  // debugging, while removing the dynamic_cast hit in release.
  assert( dynamic_pointer_cast< const TypedStructuredArray< T > >( untyped ) );
  return static_pointer_cast< const TypedStructuredArray< T > >( untyped );
}

template< typename T >
T& UniformDataItem::atIndex( std::size_t index ) {
  return (*typedArray< T >())[index];
}

template< typename T >
const T& UniformDataItem::atIndex( std::size_t index ) const {
  return (*typedArray< T >())[index];
}

template< typename T >
const T& UniformDataItem::atLocation( const xdm::DataShape<>& location ) const {
  // Validate the input.
  assert( validateBounds( location ) );
  // Compute the linear index.
  return atIndex< T >( contiguousIndex( location, mDataspace ) );
}

template< typename T >
T& UniformDataItem::atLocation( const xdm::DataShape<>& location ) {
  return const_cast< T& >(
    static_cast< const UniformDataItem& >(*this).atLocation< T >( location )
  );
}

template< typename T >
const T& UniformDataItem::atLocation( std::size_t i ) const {
  return atLocation< T >( xdm::makeShape( i ) );
}
template< typename T >
T& UniformDataItem::atLocation( std::size_t i ) {
  return const_cast< T& >(
    static_cast< const UniformDataItem& >(*this).atLocation< T >( i )
  );
}

template< typename T >
const T& UniformDataItem::atLocation( std::size_t i, std::size_t j ) const {
  return atLocation< T >( xdm::makeShape( i, j ) );
}
template< typename T >
T& UniformDataItem::atLocation( std::size_t i, std::size_t j ) {
  return const_cast< T& >(
    static_cast< const UniformDataItem& >(*this).atLocation< T >( i, j )
  );
}

template< typename T >
const T& UniformDataItem::atLocation(
  std::size_t i,
  std::size_t j,
  std::size_t k ) const {
  return atLocation< T >( xdm::makeShape( i, j, k ) );
}
template< typename T >
T& UniformDataItem::atLocation(
  std::size_t i,
  std::size_t j,
  std::size_t k ) {
  return const_cast< T& >(
    static_cast< const UniformDataItem& >(*this).atLocation< T >( i, j, k )
  );
}

template< typename T >
const T& UniformDataItem::atLocation(
  std::size_t i,
  std::size_t j,
  std::size_t k,
  std::size_t l ) const {
  return atLocation< T >( xdm::makeShape( i, j, k, l ) );
}
template< typename T >
T& UniformDataItem::atLocation(
  std::size_t i,
  std::size_t j,
  std::size_t k,
  std::size_t l ) {
  return const_cast< T& >(
    static_cast< const UniformDataItem& >(*this).atLocation< T >( i, j, k, l )
  );
}

XDM_NAMESPACE_END

#endif // xdm_UniformDataItem_hpp

