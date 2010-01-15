#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmComm/BinaryOStream.hpp>

#include <memory>

// define a test parameterized on the types we care about.
template< typename T >
class BinaryOStreamTest : public ::testing::Test {
public:
  T value;
  xdmComm::BinaryStreamBuffer mBuf;
  xdmComm::BinaryOStream mOstr;
  BinaryOStreamTest() : value(), mBuf( 512 ), mOstr( &mBuf ) {}
};
TYPED_TEST_CASE_P( BinaryOStreamTest );

TYPED_TEST_P( BinaryOStreamTest, writevalue ) {
  this->mOstr << this->value;
  
  TypeParam* result = reinterpret_cast< TypeParam* >( this->mBuf.pointer() );
  ASSERT_EQ( this->value, *result );
}

REGISTER_TYPED_TEST_CASE_P( BinaryOStreamTest, writevalue );

typedef ::testing::Types< 
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
INSTANTIATE_TYPED_TEST_CASE_P( Write, BinaryOStreamTest, WriteTypes );

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

