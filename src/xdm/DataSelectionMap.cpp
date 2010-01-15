//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdm/DataSelectionMap.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataSelection.hpp>

XDM_NAMESPACE_BEGIN

DataSelectionMap::DataSelectionMap() :
  mDomain( new AllDataSelection ),
  mRange( new AllDataSelection ) {
}

DataSelectionMap::DataSelectionMap( 
  DataSelection* domain, 
  DataSelection* range ) :
  mDomain( domain ),
  mRange( range ) {
}

DataSelectionMap::~DataSelectionMap() {
}

const DataSelection* DataSelectionMap::domain() const {
  return mDomain;
}

void DataSelectionMap::setDomain( DataSelection* domain ) {
  mDomain = domain;
}

const DataSelection* DataSelectionMap::range() const {
  return mRange;
}

void DataSelectionMap::setRange( DataSelection* range ) {
  mRange = range;
}

XDM_NAMESPACE_END

