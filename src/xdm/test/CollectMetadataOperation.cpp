#include <gtest/gtest.h>

#include <xdm/CollectMetadataOperation.hpp>

#include <xdm/Item.hpp>

#include <algorithm>

struct AggregateItem : xdm::Item {
  std::vector< xdm::RefPtr< Item > > mItems;
  void appendChild( xdm::Item* i ) { mItems.push_back( i ); }
  virtual void traverse( xdm::ItemVisitor& iv ) {
    std::for_each( mItems.begin(), mItems.end(), xdm::ApplyVisitor( iv ) );
  }
};

TEST( CollectMetadataOperation, applyItem ) {
  xdm::CollectMetadataOperation op;
  xdm::Item i;

  i.accept( op );

  xdm::RefPtr< xdm::XmlObject > opResult = op.result();

  char const * const answer = 
    "<Item>\n"
    "</Item>\n";
  std::stringstream result;
  result << *opResult;
  ASSERT_EQ( answer, result.str() );
}

TEST( CollectMetadataOperation, applyAggregateItem ) {
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
  ASSERT_EQ( answer, result.str() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

