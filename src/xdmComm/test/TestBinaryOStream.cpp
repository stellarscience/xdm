#define BOOST_TEST_MODULE BinaryOStream 
#include <boost/test/unit_test.hpp>

#include <xdmComm/BinaryOStream.hpp>

#include <memory>

// define a test parameterized on the types we care about.
template< typename T >
class Fixture {
public:
  T value;
  xdmComm::BinaryStreamBuffer mBuf;
  xdmComm::BinaryOStream mOstr;
  Fixture() : value(), mBuf( 512 ), mOstr( &mBuf ) {}
};

template< typename T > void readValue() {
  Fixture< T > test;

  test.mOstr << test.value;
  
  T* result = reinterpret_cast< T* >( test.mBuf.pointer() );
  BOOST_CHECK_EQUAL( test.value, *result );
}

BOOST_AUTO_TEST_CASE( writeChar ) { readValue< char >(); }
BOOST_AUTO_TEST_CASE( writeShort ) { readValue< short >(); }
BOOST_AUTO_TEST_CASE( writeInt ) { readValue< int >(); }
BOOST_AUTO_TEST_CASE( writeLong ) { readValue< long >(); }

BOOST_AUTO_TEST_CASE( writeUShort ) { readValue< unsigned short >(); }
BOOST_AUTO_TEST_CASE( writeUInt ) { readValue< unsigned int >(); }
BOOST_AUTO_TEST_CASE( writeULong ) { readValue< unsigned long >(); }

BOOST_AUTO_TEST_CASE( writeFloat ) { readValue< float >(); }
BOOST_AUTO_TEST_CASE( writeDouble ) { readValue< double >(); }
BOOST_AUTO_TEST_CASE( writeLongDouble ) { readValue< long double >(); }

BOOST_AUTO_TEST_CASE( writeBool ) { readValue< bool >(); }

