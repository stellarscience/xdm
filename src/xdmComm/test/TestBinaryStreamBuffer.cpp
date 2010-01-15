#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmComm/BinaryStreamBuffer.hpp>

namespace {

struct Fixture {
  xdmComm::BinaryStreamBuffer testBuffer;
  Fixture() : testBuffer( 512 ) {}
};

BOOST_AUTO_TEST_CASE( construct ) {
  Fixture test;

  BOOST_CHECK_EQUAL( 512, test.testBuffer.size() );
}

BOOST_AUTO_TEST_CASE( putgetc ) {
  Fixture test;

  test.testBuffer.sputc( 'a' );
  test.testBuffer.sputc( 'b' );
  test.testBuffer.sputc( 'c' );

  test.testBuffer.pubsync();

  char result;
  result = test.testBuffer.sgetc();
  BOOST_CHECK_EQUAL( 'a', result );
  
  result = test.testBuffer.sgetc();
  BOOST_CHECK_EQUAL( 'b', result );
  
  result = test.testBuffer.sgetc();
  BOOST_CHECK_EQUAL( 'c', result );
}

BOOST_AUTO_TEST_CASE( putgetn ) {
  Fixture test;

  char characters[] = {'a', 'b', 'c'};
  
  test.testBuffer.sputn( characters, 3 );

  test.testBuffer.pubsync();

  char result[3];
  test.testBuffer.sgetn( result, 3 );

  BOOST_CHECK_EQUAL( 'a', result[0] );
  BOOST_CHECK_EQUAL( 'b', result[1] );
  BOOST_CHECK_EQUAL( 'c', result[2] );
}

} // namespace

