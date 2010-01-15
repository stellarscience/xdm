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

  /// Open a new stream context given an XML object.  If the object is an
  /// aggregation of other objects (has children), multiple contexts will be
  /// opened leading to the final child of the innermost object.
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

