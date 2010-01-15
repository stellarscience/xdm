#include <gtest/gtest.h>

#include <xdmComm/BinaryIOStream.hpp>
#include <xdmComm/BinaryStreamOperations.hpp>

#include <algorithm>
#include <vector>

#include <cstdlib>

class BinaryStreamOperationsTest : public ::testing::Test {
public:
  xdmComm::BinaryStreamBuffer buffer;
  xdmComm::BinaryIOStream stream;
  BinaryStreamOperationsTest() :
    buffer( 1024 ),
    stream( &buffer )
  {}
};

TEST_F( BinaryStreamOperationsTest, DataShapeRoundtrip ) {
  xdm::DataShape<> answer( 4 );
  answer[0] = 1;
  answer[1] = 2;
  answer[2] = 3;
  answer[3] = 4;

  stream << answer << xdmComm::flush;

  xdm::DataShape<> result;
  stream >> result;

  ASSERT_EQ( answer, result );
}

TEST_F( BinaryStreamOperationsTest, PrimitiveTypeRoundtrip ) {
  xdm::primitiveType::Value answer( xdm::primitiveType::kInt );

  stream << answer << xdmComm::flush;

  xdm::primitiveType::Value result;
  stream >> result;
  ASSERT_EQ( answer, result );
}

TEST_F( BinaryStreamOperationsTest, StructuredArrayRoundtrip ) {
  std::vector< int > inData( 10 );
  std::generate( inData.begin(), inData.end(), rand );
  xdm::StructuredArray answer( xdm::primitiveType::kInt, &inData[0],
    xdm::makeShape( 10 ) );

  stream << answer << xdmComm::flush;

  std::vector< int > outData( 10 );
  xdm::StructuredArray result( xdm::primitiveType::kChar, &outData[0],
    xdm::DataShape<>() );
  stream >> result;
  
  ASSERT_EQ( answer.dataType(), result.dataType() );
  ASSERT_EQ( answer.shape(), result.shape() );
  ASSERT_TRUE( std::equal( inData.begin(), inData.end(), outData.begin() ) );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

