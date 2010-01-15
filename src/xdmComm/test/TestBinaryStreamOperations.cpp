#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmComm/BinaryIOStream.hpp>
#include <xdmComm/BinaryStreamOperations.hpp>

#include <xdm/DataSelectionVisitor.hpp>

#include <algorithm>
#include <vector>

#include <cstdlib>

// helper class to check if two DataSelection subclasses are equal.
template< typename T >
struct CheckDataSelectionSubclassesEqual : 
  public xdm::DataSelectionVisitor {

  T* answer;
  bool result;

  CheckDataSelectionSubclassesEqual( T* truth ) : answer( truth ) {}

  void apply( const xdm::DataSelection& ) { result = false; }
  void apply( const T& selection ) {
    result = ( selection == *answer );
  }
};

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

TEST_F( BinaryStreamOperationsTest, HyperSlabRoundtrip ) {
  xdm::HyperSlab<> answer( xdm::makeShape( 3, 3, 3 ) );

  stream << answer << xdmComm::flush;

  xdm::HyperSlab<> result;
  stream >> result;
  ASSERT_EQ( answer, result );
}

TEST_F( BinaryStreamOperationsTest, HyperslabDataSelectionRoundtrip ) {
  xdm::HyperslabDataSelection answer( 
    xdm::HyperSlab<>( xdm::makeShape( 3, 3, 3 ) ) );

  stream << answer << xdmComm::flush;

  xdm::HyperslabDataSelection result;
  stream >> result;
  ASSERT_EQ( answer.hyperslab(), result.hyperslab() );
}

TEST_F( BinaryStreamOperationsTest, DataSelectionMapRoundtrip ) {
  xdm::RefPtr< xdm::AllDataSelection > answerDomain( 
    new xdm::AllDataSelection );
  xdm::RefPtr< xdm::HyperslabDataSelection > answerRange(
    new xdm::HyperslabDataSelection( 
      xdm::HyperSlab<>( xdm::makeShape( 1, 2, 3 ) ) ) );
  xdm::DataSelectionMap answer( answerDomain, answerRange );

  stream << answer << xdmComm::flush;

  xdm::DataSelectionMap result;
  stream >> result;

  // Check the domain is an AllDataSelection
  CheckDataSelectionSubclassesEqual< xdm::AllDataSelection > domainCheck( 
    answerDomain.get() );
  result.domain()->accept( domainCheck );
  ASSERT_TRUE( domainCheck.result );

  // Check that the range is a HyperslabDataSelection with the correct values
  CheckDataSelectionSubclassesEqual< xdm::HyperslabDataSelection > rangeCheck(
    answerRange.get() );
  result.range()->accept( rangeCheck );
  ASSERT_TRUE( rangeCheck.result );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

