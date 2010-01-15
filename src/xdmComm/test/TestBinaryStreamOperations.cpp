//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdm/TemplateStructuredArray.hpp>

#include <xdmComm/BinaryIOStream.hpp>
#include <xdmComm/BinaryStreamOperations.hpp>

#include <xdm/DataSelectionVisitor.hpp>

#include <algorithm>
#include <vector>

#include <cstdlib>

namespace {

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

class Fixture {
public:
  xdmComm::BinaryStreamBuffer buffer;
  xdmComm::BinaryIOStream stream;
  Fixture() :
    buffer( 1024 ),
    stream( &buffer )
  {}
};

BOOST_AUTO_TEST_CASE( DataShapeRoundtrip ) {
  Fixture test;

  xdm::DataShape<> answer( 4 );
  answer[0] = 1;
  answer[1] = 2;
  answer[2] = 3;
  answer[3] = 4;

  test.stream << answer << xdmComm::flush;

  xdm::DataShape<> result;
  test.stream >> result;

  BOOST_CHECK_EQUAL( answer, result );
}

BOOST_AUTO_TEST_CASE( PrimitiveTypeRoundtrip ) {
  Fixture test;

  xdm::primitiveType::Value answer( xdm::primitiveType::kInt );

  test.stream << answer << xdmComm::flush;

  xdm::primitiveType::Value result;
  test.stream >> result;
  BOOST_CHECK_EQUAL( answer, result );
}

BOOST_AUTO_TEST_CASE( StructuredArrayRoundtrip ) {
  Fixture test;

  std::vector< int > inData( 10 );
  std::generate( inData.begin(), inData.end(), rand );
  xdm::TemplateStructuredArray< int > answer( &inData[0], 10 );

  test.stream << answer << xdmComm::flush;

  std::vector< int > outData( 10 );
  xdmComm::ReceiveBufferArray result( 128 );
  test.stream >> result;
  
  BOOST_CHECK_EQUAL( answer.dataType(), result.dataType() );
  BOOST_CHECK_EQUAL( answer.size(), result.size() );

  const int * answerArray = reinterpret_cast< const int * >( answer.data() );
  const int * resultArray = reinterpret_cast< const int* >( result.data() );
  BOOST_CHECK( std::equal( answerArray, answerArray + 10, resultArray ) );
}

BOOST_AUTO_TEST_CASE( HyperSlabRoundtrip ) {
  Fixture test;

  xdm::HyperSlab<> answer( xdm::makeShape( 3, 3, 3 ) );

  test.stream << answer << xdmComm::flush;

  xdm::HyperSlab<> result;
  test.stream >> result;
  BOOST_CHECK_EQUAL( answer, result );
}

BOOST_AUTO_TEST_CASE( HyperslabDataSelectionRoundtrip ) {
  Fixture test;
  
  // You might have noticed the extra set of parentheses below...This is to deal
  // with a GCC 3.2 error: For some reason, GCC 3.2 thinks the declaration below
  // is declaring a function pointer. The extra set of parentheses forces the
  // compiler to evaluate the call inside of the answer constructor first, and
  // then the compiler knows it should be calling a constructor, not declaring a
  // function pointer.
  xdm::HyperslabDataSelection answer(( 
    xdm::HyperSlab<>( xdm::makeShape( 3, 3, 3 ) ) ));

  test.stream << answer << xdmComm::flush;

  xdm::HyperslabDataSelection result;
  test.stream >> result;
  BOOST_CHECK_EQUAL( answer.hyperslab(), result.hyperslab() );
}

BOOST_AUTO_TEST_CASE( DataSelectionMapRoundtrip ) {
  Fixture test;
  
  xdm::RefPtr< xdm::AllDataSelection > answerDomain( 
    new xdm::AllDataSelection );
  xdm::RefPtr< xdm::HyperslabDataSelection > answerRange(
    new xdm::HyperslabDataSelection( 
      xdm::HyperSlab<>( xdm::makeShape( 1, 2, 3 ) ) ) );
  xdm::DataSelectionMap answer( answerDomain, answerRange );

  test.stream << answer << xdmComm::flush;

  xdm::DataSelectionMap result;
  test.stream >> result;

  // Check the domain is an AllDataSelection
  CheckDataSelectionSubclassesEqual< xdm::AllDataSelection > domainCheck( 
    answerDomain.get() );
  result.domain()->accept( domainCheck );
  BOOST_CHECK( domainCheck.result );

  // Check that the range is a HyperslabDataSelection with the correct values
  CheckDataSelectionSubclassesEqual< xdm::HyperslabDataSelection > rangeCheck(
    answerRange.get() );
  result.range()->accept( rangeCheck );
  BOOST_CHECK( rangeCheck.result );
}

} // namespace

