//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmFormat_Reader_hpp
#define xdmFormat_Reader_hpp

#include <xdm/FileSystem.hpp>
#include <xdm/Forward.hpp>
#include <xdm/ReferencedObject.hpp>

#include <sstream>
#include <stdexcept>

#include <xdmFormat/NamespaceMacro.hpp>

XDM_FORMAT_NAMESPACE_BEGIN

/// Exception to be thrown when an error occurs while reading an input file.
class ReadError : public std::runtime_error {
public:
  ReadError( const std::string& message ) :
    std::runtime_error( message.c_str() ) {}
  virtual ~ReadError() throw() {}
};

/// Exception to be thrown when an error is encountered while parsing a text
/// input file. Contains information about the line and column number of the
/// parse error.
class ParseError : public ReadError {
  int mLine;
  int mColumn;
public:
  ParseError( int line, int column ) :
    ReadError( "Parse error:" ),
    mLine( line ),
    mColumn( column ) {}
  virtual ~ParseError() throw() {}
  virtual const char* what() const throw() {
    try {
      std::stringstream ss;
      ss << ReadError::what() << "Line " << mLine << ", Column" << mColumn;
      return ss.str().c_str();
    } catch ( ... ) {
      return "Parse error";
    }
  }
  int line() const { return mLine; }
  int column() const { return mColumn; }
};

/// Interface for types that read items from files. Implement this interface to
/// read a tree of items from a file.
class Reader : public xdm::ReferencedObject {
public:
  Reader();
  virtual ~Reader();

  /// Read an Item from the file with the given path.
  /// @param path Path to the file to read.
  /// @return A new Item on success, an invalid RefPtr on failure.
  virtual xdm::RefPtr< xdm::Item >
  readItem( const xdm::FileSystemPath& path ) = 0;

  /// Update an existing Item and it's subtree for a new time step, if possible.
  /// @pre The input item was read eariler with readItem.
  /// @post The data in the input item is updated to reflect a new time step.
  /// @param item An existing item tree.
  /// @return True if new data is available, false otherwise.
  virtual bool update(
    xdm::RefPtr< xdm::Item > item,
    const xdm::FileSystemPath& path,
    std::size_t timeStep = 0 ) = 0;

};

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_Reader_hpp
