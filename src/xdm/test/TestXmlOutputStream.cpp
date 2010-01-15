#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdm/XmlObject.hpp>
#include <xdm/XmlOutputStream.hpp>

#include <sstream>

using xdm::RefPtr;
using xdm::XmlObject;
using xdm::XmlOutputStream;

BOOST_AUTO_TEST_CASE( openContext ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );
  
  std::stringstream result;
  XmlOutputStream test( result );
  test.openContext( obj );
  
  char const * const answer = "<obj>\n";
  BOOST_CHECK_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( writeSingleObject ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );

  std::stringstream result;
  XmlOutputStream test( result );
  test.writeObject( obj );

  char const * const answer =
    "<obj>\n"
    "</obj>\n";
  BOOST_CHECK_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( writeStreamingObject ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );
  RefPtr< XmlObject > chi( new XmlObject( "chi" ) );

  std::stringstream result;
  XmlOutputStream test( result );
  test.openContext( obj );
  test.writeObject( chi );

  char const * const answer_incomplete = 
    "<obj>\n"
    "  <chi>\n"
    "  </chi>\n";
  BOOST_CHECK_EQUAL( answer_incomplete, result.str() );

  test.closeCurrentContext();

  char const * const answer_complete =
    "<obj>\n"
    "  <chi>\n"
    "  </chi>\n"
    "</obj>\n";
  BOOST_CHECK_EQUAL( answer_complete, result.str() );
}

BOOST_AUTO_TEST_CASE( closeCurrentContext ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );

  std::stringstream result;
  XmlOutputStream test( result );
  test.openContext( obj );
  test.closeCurrentContext();

  char const * const answer = 
    "<obj>\n"
    "</obj>\n";
  BOOST_CHECK_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( closeStream ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );
  RefPtr< XmlObject > chi( new XmlObject( "chi" ) );

  std::stringstream result;
  XmlOutputStream test( result );
  
  test.openContext( obj );
  test.openContext( chi );
  test.closeStream();
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

