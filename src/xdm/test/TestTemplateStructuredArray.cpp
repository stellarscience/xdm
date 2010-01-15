#define BOOST_TEST_MODULE TemplateStructuredArray
#include <boost/test/unit_test.hpp>

#include <xdm/DataShape.hpp>
#include <xdm/TemplateStructuredArray.hpp>
#include <xdm/RefPtr.hpp>

namespace {

template< typename T > 
void test() {
  T value = T();
  xdm::RefPtr< xdm::TemplateStructuredArray< T > > array( 
    new xdm::TemplateStructuredArray< T >( &value, 1 ) );
  T* result = array->begin();
  BOOST_CHECK_EQUAL( result, &value );
}

BOOST_AUTO_TEST_CASE( instantiateChar ) { test< char >(); } 
BOOST_AUTO_TEST_CASE( instantiateShort ) { test< short >(); } 
BOOST_AUTO_TEST_CASE( instantiateInt ) { test< int >(); } 
BOOST_AUTO_TEST_CASE( instantiateLongInt ) { test< long int >(); } 

BOOST_AUTO_TEST_CASE( instantiateUChar ) { test< unsigned char >(); } 
BOOST_AUTO_TEST_CASE( instantiateUShort ) { test< unsigned short >(); } 
BOOST_AUTO_TEST_CASE( instantiateUInt ) { test< unsigned int >(); } 
BOOST_AUTO_TEST_CASE( instantiateULInt ) { test< long unsigned int >(); } 

BOOST_AUTO_TEST_CASE( instantiateFloat ) { test< float >(); } 
BOOST_AUTO_TEST_CASE( instantiateDouble ) { test< double >(); } 

} // namespace

