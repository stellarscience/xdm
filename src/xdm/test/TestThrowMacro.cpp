#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdm/ThrowMacro.hpp>

#include <stdexcept>

BOOST_AUTO_TEST_CASE( throws ) {
  ASSERT_THROW( XDM_THROW( std::runtime_error( "Test exception" ) ), 
    std::runtime_error );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

