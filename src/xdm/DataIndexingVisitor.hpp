//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
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
#ifndef xdm_DataIndexingVisitor_hpp
#define xdm_DataIndexingVisitor_hpp

#include <stdexcept>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class StructuredArray;

template< typename T >
class TypedStructuredArray;

class UniformDataItem;

class DataIndexingVisitor {
public:
  DataIndexingVisitor();
  virtual ~DataIndexingVisitor();

  /// Access begins at the level of UniformDataItem. This overload
  /// contains the logic that finds the proper lower level call.
  virtual void apply( UniformDataItem& item );

  /// Base class indexing access is not implemented.
  virtual void apply( StructuredArray& array );

  // Apply methods for primitive types.
  virtual void apply( TypedStructuredArray< char >& array );
  virtual void apply( TypedStructuredArray< short >& array );
  virtual void apply( TypedStructuredArray< int >& array );
  virtual void apply( TypedStructuredArray< long int >& array );
  virtual void apply( TypedStructuredArray< unsigned char >& array );
  virtual void apply( TypedStructuredArray< unsigned short >& array );
  virtual void apply( TypedStructuredArray< unsigned int >& array );
  virtual void apply( TypedStructuredArray< unsigned long int >& array );
  virtual void apply( TypedStructuredArray< float >& array );
  virtual void apply( TypedStructuredArray< double >& array );
};

class DataTypeMismatchException : public std::runtime_error {
public:
  DataTypeMismatchException( const std::string& dataType );
  ~DataTypeMismatchException() throw();
private:
  std::string mDataType;
};

XDM_NAMESPACE_END

#endif // xdm_DataIndexingVisitor_hpp

