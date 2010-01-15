#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmComm/BinaryStreamBuffer.hpp>

struct BinaryStreamBufferTest : ::testing::Test {
  xdmComm::BinaryStreamBuffer testBuffer;
  BinaryStreamBufferTest() : testBuffer( 512 ) {}
};

BOOST_AUTO_TEST_CASE( construct ) {
  BOOST_CHECK_EQUAL( 512, testBuffer.size() );
}

BOOST_AUTO_TEST_CASE( putgetc ) {
  testBuffer.sputc( 'a' );
  testBuffer.sputc( 'b' );
  testBuffer.sputc( 'c' );

  testBuffer.pubsync();

  char result;
  result = testBuffer.sgetc();
  BOOST_CHECK_EQUAL( 'a', result );
  
  result = testBuffer.sgetc();
  BOOST_CHECK_EQUAL( 'b', result );
  
  result = testBuffer.sgetc();
  BOOST_CHECK_EQUAL( 'c', result );
}

BOOST_AUTO_TEST_CASE( putgetn ) {
  char characters[] = {'a', 'b', 'c'};
  
  testBuffer.sputn( characters, 3 );

  testBuffer.pubsync();

  char result[3];
  testBuffer.sgetn( result, 3 );

  BOOST_CHECK_EQUAL( 'a', result[0] );
  BOOST_CHECK_EQUAL( 'b', result[1] );
  BOOST_CHECK_EQUAL( 'c', result[2] );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

