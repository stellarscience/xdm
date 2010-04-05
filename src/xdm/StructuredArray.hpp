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
#ifndef xdm_StructuredArray_hpp
#define xdm_StructuredArray_hpp

#include <xdm/DataShape.hpp>
#include <xdm/PrimitiveType.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/SelectableDataMixin.hpp>

#include <xdm/NamespaceMacro.hpp>

#include <sstream>
#include <stdexcept>
#include <vector>

XDM_NAMESPACE_BEGIN

/// Exception signalling that there is not enough memory to hold data with the
/// given size.
class NotEnoughMemoryError : public std::runtime_error {
  size_t mRequested;
public:
  NotEnoughMemoryError( size_t requested ) :
    std::runtime_error( "Not enough memory for this operation" ),
    mRequested( requested ) {}
  virtual const char* what() const throw () {
    try {
      std::stringstream ss;
      ss << "Not enough memory for requested " << mRequested << " bytes";
      return ss.str().c_str();
    } catch ( ... ) {
      return std::runtime_error::what();
    }
  }
};

/// Array class for holding multiple types of data. This class represents a
/// contiguous array of data in memory.  It does not own the data it holds, it
/// merely presents an interface for querying information about an array that is
/// useful when reading or writing data.  The TypedStructuredArray subclass
/// implements a type safe interface for providing this information.
///
/// This class does not own the data it references, it is up to subclasses to
/// to manage (or not manage) memory. This is to allow applications to wrap
/// a StructuredArray around existing memory structures.
///
/// @see TypedStructuredArray
/// @see VectorStructuredArray
class StructuredArray : public ReferencedObject {
public:

  StructuredArray();
  virtual ~StructuredArray();

  /// Get the type of the data.
  virtual primitiveType::Value dataType() const = 0;

  /// Get the size of the array elements in bytes.
  virtual size_t elementSize() const = 0;

  /// Get the number of data elements.
  virtual size_t size() const = 0;

  /// Get a const pointer to the actual data.
  virtual const void* data() const = 0;

  /// Get a pointer to the array data. The default implementation uses the
  /// const qualified version, although it is virtual in case subclasses need
  /// another implementation.
  ///
  /// The non-const version calling the const version is an idiom described in
  /// Myers' Effective C++ Third Edition p. 24. It is very important that the
  /// non-const version call the const version and not the other way around.
  virtual void* data() {
    return const_cast< void* >(
      static_cast< const StructuredArray& >(*this).data() );
  }

  /// Get the size of the array in memory in bytes.
  size_t memorySize() const;

  /// Resize the array to hold the specified number of data elements. This
  /// method should have the same semantics as std::vector. That is, a call to
  /// resize should only ever result in increased memory use. If the requested
  /// size is less than the current size, subsequent calls to size() should
  /// return the requested size, but no memory should be returned to the system.
  /// @param count The number of data elements to hold.
  /// @throw OutOfMemoryError There is not enough memory for the operation.
  virtual void resize( size_t count ) = 0;

};

XDM_NAMESPACE_END

#endif // xdm_StructuredArray_hpp

