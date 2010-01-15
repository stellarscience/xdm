#include <gtest/gtest.h>

#include <xdm/DataShape.hpp>

#include <sstream>

typedef xdm::DataShape<> TestShape;

TEST( DataShape, OStreamInserter ) {
  TestShape test( 4 );
  for ( int i = 0; i < 4; ++i ) {
    test[i] = i+1;
  }

  char const * const answer = "1 2 3 4";
  std::stringstream result;
  result << test;
  ASSERT_EQ( answer, result.str() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
