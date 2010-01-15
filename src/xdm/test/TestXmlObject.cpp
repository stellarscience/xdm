#define BOOST_TEST_MODULE XmlObject 
#include <boost/test/unit_test.hpp>

#include <xdm/XmlObject.hpp>

#include <sstream>

namespace {

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

BOOST_AUTO_TEST_CASE( printHeader ) {
  xdm::RefPtr< XmlObject > obj( new XmlObject( "obj" ) );

  char const * const answer = "<obj>\n";

  std::stringstream result;
  obj->printHeader( result );

  BOOST_CHECK_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( printTextContent ) {
  xdm::RefPtr< XmlObject > obj( new XmlObject( "obj" ) );
  obj->appendContent( "text content" );
  BOOST_CHECK_EQUAL( "text content", obj->contentLine(0) );

  char const * const answer = "text content\n";

  std::stringstream result;
  obj->printTextContent( result );
  obj->printChildren( result );

  BOOST_CHECK_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( OStreamInsertion ) {
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
  BOOST_CHECK_EQUAL( answer, result.str() );
}

} // namespace anon

