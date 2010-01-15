#define BOOST_TEST_MODULE BinaryIStream 
#include <boost/test/unit_test.hpp>

#include <xdmComm/BinaryIStream.hpp>

#include <memory>

namespace {

// define a test parameterized on the types we care about.
template< typename T >
class Fixture {
public:
  T value;
  xdmComm::BinaryStreamBuffer mBuf;
  xdmComm::BinaryIStream mIStr;
  Fixture() : value(), mBuf( 512 ), mIStr( &mBuf ) {
    mBuf.sputn( reinterpret_cast< char* >(&value), sizeof(T) );
    mBuf.pubsync();
  }
};

template< typename T > void writeValue() {
  Fixture< T > test;

  T result;
  // make sure result isn't the default object
  result = !result;

  test.mIStr >> result;
  BOOST_CHECK_EQUAL( test.value, result );
}

BOOST_AUTO_TEST_CASE( writeChar ) { writeValue< char >(); }
BOOST_AUTO_TEST_CASE( writeShort ) { writeValue< short >(); }
BOOST_AUTO_TEST_CASE( writeInt ) { writeValue< int >(); }
BOOST_AUTO_TEST_CASE( writeLong ) { writeValue< long >(); }

BOOST_AUTO_TEST_CASE( writeUShort ) { writeValue< unsigned short >(); }
BOOST_AUTO_TEST_CASE( writeUInt ) { writeValue< unsigned int >(); }
BOOST_AUTO_TEST_CASE( writeULong ) { writeValue< unsigned long >(); }

BOOST_AUTO_TEST_CASE( writeFloat ) { writeValue< float >(); }
BOOST_AUTO_TEST_CASE( writeDouble ) { writeValue< double >(); }
BOOST_AUTO_TEST_CASE( writeLongDouble ) { writeValue< long double >(); }

BOOST_AUTO_TEST_CASE( writeBool ) { writeValue< bool >(); }

} // namepsace anon

