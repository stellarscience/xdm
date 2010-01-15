#define BOOST_TEST_MODULE BinaryIOStream 
#include <boost/test/unit_test.hpp>

#include <xdmComm/BinaryIOStream.hpp>

#include <memory>

namespace {

// define a test parameterized on the types we care about.
template< typename T >
class Fixture {
public:
  xdmComm::BinaryStreamBuffer mBuf;
  xdmComm::BinaryIOStream mIOStr;
  Fixture() : mBuf( 512 ), mIOStr( &mBuf ) {
  }
};

template< typename T > void writeReadTest() {
  Fixture< T > test;
  
  T answer = T();

  test.mIOStr << answer;

  test.mIOStr.flush();

  T result = T();
  result = !result; // ensure result is not default value for a good test
  test.mIOStr >> result;

  BOOST_CHECK_EQUAL( answer, result );
}

BOOST_AUTO_TEST_CASE( writeReadChar ) { writeReadTest< char >(); }
BOOST_AUTO_TEST_CASE( writeReadShort ) { writeReadTest< short >(); }
BOOST_AUTO_TEST_CASE( writeReadInt ) { writeReadTest< int >(); }
BOOST_AUTO_TEST_CASE( writeReadLong ) { writeReadTest< long >(); }

BOOST_AUTO_TEST_CASE( writeReadUShort ) { writeReadTest< unsigned short >(); }
BOOST_AUTO_TEST_CASE( writeReadUInt ) { writeReadTest< unsigned int >(); }
BOOST_AUTO_TEST_CASE( writeReadULong ) { writeReadTest< unsigned long >(); }

BOOST_AUTO_TEST_CASE( writeReadFloat ) { writeReadTest< float >(); }
BOOST_AUTO_TEST_CASE( writeReadDouble ) { writeReadTest< double >(); }
BOOST_AUTO_TEST_CASE( writeReadLongDouble ) { writeReadTest< long double >(); }

BOOST_AUTO_TEST_CASE( writeReadBool ) { writeReadTest< bool >(); }

} // namespace anon

