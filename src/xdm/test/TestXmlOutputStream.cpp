#include <gtest/gtest.h>

#include <xdm/XmlObject.hpp>
#include <xdm/XmlOutputStream.hpp>

#include <sstream>

using xdm::RefPtr;
using xdm::XmlObject;
using xdm::XmlOutputStream;

TEST( XmlOutputStream, openContext ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );
  
  std::stringstream result;
  XmlOutputStream test( result );
  test.openContext( obj );
  
  char const * const answer = "<obj>\n";
  ASSERT_EQ( answer, result.str() );
}

TEST( XmlOutputStream, writeSingleObject ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );

  std::stringstream result;
  XmlOutputStream test( result );
  test.writeObject( obj );

  char const * const answer =
    "<obj>\n"
    "</obj>\n";
  ASSERT_EQ( answer, result.str() );
}

TEST( XmlOutputStream, writeStreamingObject ) {
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
  ASSERT_EQ( answer_incomplete, result.str() );

  test.closeCurrentContext();

  char const * const answer_complete =
    "<obj>\n"
    "  <chi>\n"
    "  </chi>\n"
    "</obj>\n";
  ASSERT_EQ( answer_complete, result.str() );
}

TEST( XmlOutputStream, closeCurrentContext ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );

  std::stringstream result;
  XmlOutputStream test( result );
  test.openContext( obj );
  test.closeCurrentContext();

  char const * const answer = 
    "<obj>\n"
    "</obj>\n";
  ASSERT_EQ( answer, result.str() );
}

TEST( XmlOutputStream, closeStream ) {
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

