#include <gtest/gtest.h>

#include <xdm/CollectMetadataOperation.hpp>

#include <xdm/Item.hpp>

TEST( CollectMetadataVisitor, applyItem ) {
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

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

