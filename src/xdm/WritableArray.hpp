//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_WritableArray_hpp
#define xdm_WritableArray_hpp

#include <xdm/DataSelectionMap.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/WritableData.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class Dataset;
class StructuredArray;

/// Writes an Array with a given selection to a selection within a dataset.
class WritableArray : public WritableData {
public:
  WritableArray( StructuredArray* array, bool isDynamic = false );
  virtual ~WritableArray();

  StructuredArray* array();
  const StructuredArray* array() const;
  void setArray( StructuredArray* array );

  const DataSelectionMap& selectionMap() const;
  void setSelectionMap( const DataSelectionMap& selectionMap );

  virtual void writeImplementation( Dataset* dataset );

private:
  RefPtr< StructuredArray > mArray;
  DataSelectionMap mSelectionMap;
};

XDM_NAMESPACE_END

#endif // xdm_WritableArray_hpp

