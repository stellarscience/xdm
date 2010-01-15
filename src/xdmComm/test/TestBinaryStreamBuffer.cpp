#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmComm/BinaryStreamBuffer.hpp>

struct BinaryStreamBufferTest : ::testing::Test {
  xdmComm::BinaryStreamBuffer testBuffer;
  BinaryStreamBufferTest() : testBuffer( 512 ) {}
};

TEST_F( BinaryStreamBufferTest, construct ) {
  ASSERT_EQ( 512, testBuffer.size() );
}

TEST_F( BinaryStreamBufferTest, putgetc ) {
  testBuffer.sputc( 'a' );
  testBuffer.sputc( 'b' );
  testBuffer.sputc( 'c' );

  testBuffer.pubsync();

  char result;
  result = testBuffer.sgetc();
  ASSERT_EQ( 'a', result );
  
  result = testBuffer.sgetc();
  ASSERT_EQ( 'b', result );
  
  result = testBuffer.sgetc();
  ASSERT_EQ( 'c', result );
}

TEST_F( BinaryStreamBufferTest, putgetn ) {
  char characters[] = {'a', 'b', 'c'};
  
  testBuffer.sputn( characters, 3 );

  testBuffer.pubsync();

  char result[3];
  testBuffer.sgetn( result, 3 );

  ASSERT_EQ( 'a', result[0] );
  ASSERT_EQ( 'b', result[1] );
  ASSERT_EQ( 'c', result[2] );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

