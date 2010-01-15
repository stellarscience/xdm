//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE CollectMetadataOperation 
#include <boost/test/unit_test.hpp>

#include <xdm/CollectMetadataOperation.hpp>

#include <xdm/Item.hpp>

#include <algorithm>

namespace {

struct AggregateItem : xdm::Item {
  std::vector< xdm::RefPtr< Item > > mItems;
  void appendChild( xdm::Item* i ) { mItems.push_back( i ); }
  virtual void traverse( xdm::ItemVisitor& iv ) {
    std::for_each( mItems.begin(), mItems.end(), xdm::ApplyVisitor( iv ) );
  }
};

BOOST_AUTO_TEST_CASE( applyItem ) {
  xdm::CollectMetadataOperation op;
  xdm::Item i;

  i.accept( op );

  xdm::RefPtr< xdm::XmlObject > opResult = op.result();

  char const * const answer = 
    "<Item>\n"
    "</Item>\n";
  std::stringstream result;
  result << *opResult;
  BOOST_REQUIRE_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( applyAggregateItem ) {
  xdm::RefPtr< AggregateItem > item[4];
  item[0] = new AggregateItem;
  for ( int i = 1; i < 4; ++i ) {
    item[i] = new AggregateItem;
    item[i-1]->appendChild( item[i] );
  }

  xdm::CollectMetadataOperation op;
  item[0]->accept( op );
  xdm::RefPtr< xdm::XmlObject > opResult = op.result();

  char const * const answer = 
    "<Item>\n"
    "  <Item>\n"
    "    <Item>\n"
    "      <Item>\n"
    "      </Item>\n"
    "    </Item>\n"
    "  </Item>\n"
    "</Item>\n";
  std::stringstream result;
  result << *opResult;
  BOOST_REQUIRE_EQUAL( answer, result.str() );
}

} // namespace

