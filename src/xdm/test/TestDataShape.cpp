#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdm/DataShape.hpp>

#include <sstream>
#include <stdexcept>

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

TEST( DataShape, makeShape ) {
  TestShape result = xdm::makeShape( "1 2 3 4" );
  ASSERT_EQ( 4, result.rank() );
  ASSERT_EQ( 1, result[0] );
  ASSERT_EQ( 2, result[1] );
  ASSERT_EQ( 3, result[2] );
  ASSERT_EQ( 4, result[3] );

  ASSERT_THROW( xdm::makeShape( "1 jeff" ), std::invalid_argument );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
