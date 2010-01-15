#include <xdm/XmlOutputStream.hpp>

#include <xdm/XmlObject.hpp>

#include <algorithm>

#include <cassert>

XDM_NAMESPACE_BEGIN

namespace {

struct WriteFormattedXml {
  std::ostream& mOStr;
  int mIndent;
  WriteFormattedXml( std::ostream& ostr, int indent ) :
    mOStr( ostr ),
    mIndent( indent ) {}
  void operator()( RefPtr< XmlObject > obj ) {
    writeIndent( mOStr, *obj, mIndent );
  }
};

} // namespace anon

XmlOutputStream::XmlOutputStream( std::ostream& output ) :
  mOutput( output ),
  mContextStack() {
}

XmlOutputStream::~XmlOutputStream() {
  closeStream();
}

void XmlOutputStream::openContext( RefPtr< XmlObject > obj ) {
  // write the header and body of the input object first.  That way the context
  // isn't modified if this operation fails.
  obj->printHeader( mOutput, mContextStack.size() );
  obj->printTextContent( mOutput, mContextStack.size() );

  // now push the object onto the context stack.
  mContextStack.push( obj );

  if( obj->hasChildren() ) {
    // write the complete body of all but the final child to the stream
    XmlObject::ChildIterator finalChild = --(obj->endChildren());
    std::for_each( obj->beginChildren(), finalChild, 
      WriteFormattedXml( mOutput, mContextStack.size() ) );

    // open a context for my final child
    openContext( *finalChild );
  }
}

void XmlOutputStream::writeObject( RefPtr< XmlObject > obj ) {
  WriteFormattedXml write( mOutput, mContextStack.size() );
  write( obj );
}

void XmlOutputStream::closeCurrentContext() {
  assert( !mContextStack.empty() );

  // Save a reference to the current top of the stack.
  RefPtr< XmlObject > top = mContextStack.top();

  // pop the stack, we follow the reverse order of openContext so that the
  // indentation level is managed by the size of the stack.
  mContextStack.pop();

  // now print the footer of the closed object to the stream.
  top->printFooter( mOutput, mContextStack.size() );
}

void XmlOutputStream::closeStream() {
  while ( !mContextStack.empty() ) {
    closeCurrentContext();
  }
}

XDM_NAMESPACE_END

