#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmComm/BinaryIOStream.hpp>

#include <memory>

// define a test parameterized on the types we care about.
template< typename T >
class BinaryIOStreamTest : public ::testing::Test {
public:
  xdmComm::BinaryStreamBuffer mBuf;
  xdmComm::BinaryIOStream mIOStr;
  BinaryIOStreamTest() : mBuf( 512 ), mIOStr( &mBuf ) {
  }
};
TYPED_TEST_CASE_P( BinaryIOStreamTest );

TYPED_TEST_P( BinaryIOStreamTest, writereadvalue ) {
  TypeParam answer = TypeParam();

  this->mIOStr << answer;

  this->mIOStr.flush();

  TypeParam result = TypeParam();
  result = !result;
  this->mIOStr >> result;

  BOOST_CHECK_EQUAL( answer, result );
}

REGISTER_TYPED_TEST_CASE_P( BinaryIOStreamTest, writereadvalue );

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
  bool > ReadWriteTypes;
INSTANTIATE_TYPED_TEST_CASE_P( ReadWrite, BinaryIOStreamTest, ReadWriteTypes );

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

