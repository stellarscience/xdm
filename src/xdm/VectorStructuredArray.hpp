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
#ifndef xdm_VectorStructuredArray_hpp
#define xdm_VectorStructuredArray_hpp

#include <xdm/TemplateStructuredArray.hpp>

#include <vector>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// StructuredArray that manages its own storage with a standard vector. The
/// lifetime of the data is tied to the lifetime of the StructuredArray.
template< typename T >
class VectorStructuredArray : public StructuredArray {
private:
  std::vector< T > mVector;

public:

  //-- STL Access Types --//

  typedef typename std::vector< T >::size_type size_type;
  typedef typename std::vector< T >::value_type value_type;
  typedef typename std::vector< T >::pointer pointer;
  typedef typename std::vector< T >::iterator iterator;
  typedef typename std::vector< T >::const_iterator const_iterator;
  typedef typename std::vector< T >::reference reference;
  typedef typename std::vector< T >::const_reference const_reference;

  /// Default constructor initializes with empty storage.
  VectorStructuredArray() :
    StructuredArray(),
    mVector() {
  }

  /// Constructor takes a size and optional initialization value for all
  /// elements. If no initialization is specified, it is filled with a
  /// defaultly constructed value.
  VectorStructuredArray( size_t size, const_reference t = value_type() ) :
    StructuredArray(),
    mVector( size, t ) {
  }

  virtual ~VectorStructuredArray() {}

  iterator begin() { return mVector.begin(); }
  const_iterator begin() const { return mVector.begin(); }
  iterator end() { return mVector.end(); }
  const_iterator end() const { return mVector.end(); }

  reference operator[]( size_type i ) { return mVector[i]; }
  const_reference operator[]( size_type i ) const { return mVector[i]; }

  void resize( size_type n ) { mVector.resize( n ); }
  void reserve( size_type n) { mVector.reserve( n ); }

  //-- StructuredArray Query Interface --//
  virtual primitiveType::Value dataType() const {
    return PrimitiveTypeInfo< T >::kValue;
  }
  virtual size_t elementSize() const {
    return PrimitiveTypeInfo< T >::kSize;
  }
  virtual size_t size() const {
    return mVector.size();
  }
  virtual const void* data() const {
    return mVector.data();
  }

};

XDM_NAMESPACE_END

#endif // xdm_VectorStructuredArray_hpp

