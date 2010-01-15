//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE Item 
#include <boost/test/unit_test.hpp>

#include <xdm/Item.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadataTag ) {
  xdm::RefPtr< xdm::Item > i( new xdm::Item );
  i->setName( "Fred" );

  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );
  i->writeMetadata( xml );

  char const * const tagAnswer = "Item";
  std::string tagResult = xml.tag();
  BOOST_CHECK_EQUAL( tagAnswer, tagResult );

  char const * const nameAnswer = "Fred";
  std::string nameResult = xml.attribute( "Name" );
  BOOST_CHECK_EQUAL( nameAnswer, nameResult );
}

} // namespace

