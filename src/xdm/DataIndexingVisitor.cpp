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
#include <xdm/DataIndexingVisitor.hpp>
#include <xdm/TemplateStructuredArray.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/WritableArray.hpp>

#include <xdm/ThrowMacro.hpp>

XDM_NAMESPACE_BEGIN

DataIndexingVisitor::DataIndexingVisitor() {
}

DataIndexingVisitor::~DataIndexingVisitor() {
}

void DataIndexingVisitor::apply( UniformDataItem& item ) {
  // This logic only applies to UniformDataItems that have a single array underneath so that
  // the data is stored contiguously and can be indexed as a simple C array.
  if ( item.writables().size() != 1 ) {
    XDM_THROW( std::runtime_error( "DataIndexingVisitor cannot determine which WritableArray"
      " to use because there are more than one in the UniformDataItem." ) );
  }

  RefPtr< WritableArray > array =
    dynamic_pointer_cast< WritableArray >( item.writables().front() );
  if ( ! array ) {
    XDM_THROW( std::runtime_error( "DataIndexingVisitor failed to find a WritableArray in"
      " the UniformDataItem." ) );
  }

  array->array()->accept( *this );
}

void DataIndexingVisitor::apply( StructuredArray& array ) {
  XDM_THROW( std::logic_error( "DataIndexingVisitor is not implemented for the base"
    " class StructuredArray." ) );
}

void DataIndexingVisitor::apply( TemplateStructuredArray< char >& array ) {
  XDM_THROW( DataTypeMismatchException( "char" ) );
}

void DataIndexingVisitor::apply( TemplateStructuredArray< short >& array ) {
  XDM_THROW( DataTypeMismatchException( "short" ) );
}

void DataIndexingVisitor::apply( TemplateStructuredArray< int >& array ) {
  XDM_THROW( DataTypeMismatchException( "int" ) );
}

void DataIndexingVisitor::apply( TemplateStructuredArray< long int >& array ) {
  XDM_THROW( DataTypeMismatchException( "long int" ) );
}

void DataIndexingVisitor::apply( TemplateStructuredArray< unsigned char >& array ) {
  XDM_THROW( DataTypeMismatchException( "unsigned char" ) );
}

void DataIndexingVisitor::apply( TemplateStructuredArray< unsigned short >& array ) {
  XDM_THROW( DataTypeMismatchException( "unsigned short" ) );
}

void DataIndexingVisitor::apply( TemplateStructuredArray< unsigned int >& array ) {
  XDM_THROW( DataTypeMismatchException( "unsigned int" ) );
}

void DataIndexingVisitor::apply( TemplateStructuredArray< unsigned long int >& array ) {
  XDM_THROW( DataTypeMismatchException( "unsigned long int" ) );
}

void DataIndexingVisitor::apply( TemplateStructuredArray< float >& array ) {
  XDM_THROW( DataTypeMismatchException( "float" ) );
}

void DataIndexingVisitor::apply( TemplateStructuredArray< double >& array ) {
  XDM_THROW( DataTypeMismatchException( "double" ) );
}

DataTypeMismatchException::DataTypeMismatchException( const std::string& dataType ) :
  std::runtime_error( "DataIndexingVisitor::apply( TemplateStructuredArray<" + dataType +
    ">& array ) was called. Typically, this means that the correct virtual call does not"
    " exist, which indicates that the desired data type does not match the data type of"
    " the array argument." ),
  mDataType( dataType ) {
}

DataTypeMismatchException::~DataTypeMismatchException() throw() {
}

XDM_NAMESPACE_END
