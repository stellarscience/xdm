#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmComm/BinaryIStream.hpp>

#include <memory>

// define a test parameterized on the types we care about.
template< typename T >
class BinaryIStreamTest : public ::testing::Test {
public:
  T value;
  xdmComm::BinaryStreamBuffer mBuf;
  xdmComm::BinaryIStream mIStr;
  BinaryIStreamTest() : value(), mBuf( 512 ), mIStr( &mBuf ) {
    mBuf.sputn( reinterpret_cast< char* >(&value), sizeof(T) );
    mBuf.pubsync();
  }
};
TYPED_TEST_CASE_P( BinaryIStreamTest );

TYPED_TEST_P( BinaryIStreamTest, writevalue ) {
  TypeParam result;
  // make sure result isn't the default object
  result = !result;

  this->mIStr >> result;
  BOOST_CHECK_EQUAL( this->value, result );
}

REGISTER_TYPED_TEST_CASE_P( BinaryIStreamTest, writevalue );

typedef ::testing::Types< 
  char,
  short,
  int,
  long,
  unsigned short,
  unsigned int,
  unsigned long,
  float,
  double,
  long double,
  bool > WriteTypes;
INSTANTIATE_TYPED_TEST_CASE_P( Read, BinaryIStreamTest, WriteTypes );

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

