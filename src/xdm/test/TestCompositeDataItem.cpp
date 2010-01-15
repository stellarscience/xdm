//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE CompositeDataItem 
#include <boost/test/unit_test.hpp>

#include <xdm/CompositeDataItem.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdm::CompositeDataItem i;
  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml(obj);

  i.writeMetadata( xml );

  BOOST_CHECK_EQUAL( xml.tag(), "DataItem" );
  BOOST_CHECK_EQUAL( xml.attribute( "ItemType" ), "Tree" );
}

} // namespace

