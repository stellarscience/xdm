//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdm/WritableArray.hpp>

#include <xdm/DataSelection.hpp>
#include <xdm/Dataset.hpp>
#include <xdm/StructuredArray.hpp>

XDM_NAMESPACE_BEGIN

WritableArray::WritableArray( StructuredArray* array, bool isDynamic ) :
  WritableData( isDynamic ),
  mArray( array ),
  mSelectionMap()
{
}

WritableArray::~WritableArray()
{
}

StructuredArray* WritableArray::array() {
  return mArray.get();
}

const StructuredArray* WritableArray::array() const {
  return mArray.get();
}

void WritableArray::setArray( StructuredArray* array ) {
  mArray = array;
}

const DataSelectionMap& WritableArray::selectionMap() const {
  return mSelectionMap;
}

void WritableArray::setSelectionMap( const DataSelectionMap& selectionMap ) {
  mSelectionMap = selectionMap;
}

void WritableArray::writeImplementation( Dataset* dataset ) {
  dataset->serialize( mArray, mSelectionMap );
}

XDM_NAMESPACE_END

