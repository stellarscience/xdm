#include <gtest/gtest.h>

#include <xdm/XmlObject.hpp>

#include <sstream>

using xdm::XmlObject;

xdm::RefPtr< XmlObject > buildTree() {
  xdm::RefPtr< XmlObject > foo( new XmlObject( "foo" ) );
  foo->appendAttribute( "bar", "42" );
  xdm::RefPtr< XmlObject > baz( new XmlObject( "baz" ) );
  foo->appendChild( baz );
  baz->appendContent( "quack" );
  baz->appendChild( new XmlObject( "quux" ) );
  return foo;
}

TEST( XmlObject, OStreamInsertion ) {
  char const * const answer = 
    "<foo bar='42'>\n"
    "  <baz>\n"
    "    quack\n"
    "    <quux>\n"
    "    </quux>\n"
    "  </baz>\n"
    "</foo>\n";
  xdm::RefPtr< XmlObject > obj = buildTree();
  std::stringstream result;
  result << *obj;
  ASSERT_EQ( answer, result.str() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

