#include <xdm/XmlOutputStream.hpp>

#include <xdm/XmlObject.hpp>

#include <cassert>

XDM_NAMESPACE_BEGIN

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

  // now push the object onto the context stack.
  mContextStack.push( obj );
}

void XmlOutputStream::writeObject( RefPtr< XmlObject > obj ) {
  writeIndent( mOutput, *obj, mContextStack.size() );
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

