#define BOOST_TEST_MODULE ThrowMacro 
#include <boost/test/unit_test.hpp>

#include <xdm/ThrowMacro.hpp>

#include <stdexcept>

BOOST_AUTO_TEST_CASE( throws ) {
  BOOST_CHECK_THROW( XDM_THROW( std::runtime_error( "Test exception" ) ), 
    std::runtime_error );
}

