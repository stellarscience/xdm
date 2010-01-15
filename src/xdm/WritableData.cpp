//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdm/WritableData.hpp>

#include <xdm/Dataset.hpp>

XDM_NAMESPACE_BEGIN

WritableData::WritableData( bool isDynamic ) : 
  ReferencedObject(),
  mIsDynamic( isDynamic ),
  mNeedsUpdate( true )
{
}

WritableData::~WritableData()
{
}

bool WritableData::isDynamic() const 
{
  return mIsDynamic;
}

void WritableData::setIsDynamic( bool isDynamic )
{
  mIsDynamic = isDynamic;
}

bool WritableData::needsUpdate() const
{
  return mNeedsUpdate;
}

void WritableData::setNeedsUpdate( bool needsUpdate )
{
  mNeedsUpdate = needsUpdate;
}

void WritableData::write( Dataset* dataset )
{
  if ( mIsDynamic || mNeedsUpdate ) {
    writeImplementation( dataset );
    mNeedsUpdate = false;
  }
}

XDM_NAMESPACE_END

