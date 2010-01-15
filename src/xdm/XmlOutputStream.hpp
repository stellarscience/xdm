#ifndef xdm_XmlOutputStream_hpp
#define xdm_XmlOutputStream_hpp

#include <xdm/RefPtr.hpp>

#include <ostream>
#include <stack>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class XmlObject;

/// A class to stream XML objects to a std::ostream.  The purpose of this class
/// is to support incremental XML output to a stream.  It allows objects to be
/// opened and closed, and for full objects to be written within the current
/// context.
class XmlOutputStream {
public:
  XmlOutputStream( std::ostream& output );
  virtual ~XmlOutputStream();

  /// Open a new new context given an XML object.  This will output the
  /// XmlObject's header and body (if it has one) to the output stream. Content
  /// fo the objects written to the stream will appear after the body of the
  /// input object.  A call to this method causes the reference count of the
  /// XmlObject to be incremented.
  ///
  /// @param obj XmlObject to open and print header and body.
  void openContext( RefPtr< XmlObject > obj );

  /// Write a a complete XML object within the current context.
  void writeObject( RefPtr< XmlObject > obj );

  /// Close the current context.  Closes the current context object and changes
  /// to the scope of the current context's parent.  When the final context is
  /// closed, the stream is closed.
  void closeCurrentContext();

  /// Close the stream, completing all open contexts.
  void closeStream();

private:
  std::ostream& mOutput;
  std::stack< RefPtr< XmlObject > > mContextStack;
};

XDM_NAMESPACE_END

#endif // xdm_XmlOutputStream_hpp

