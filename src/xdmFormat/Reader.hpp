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
#include <utility>

#include <xdmFormat/NamespaceMacro.hpp>

XDM_FORMAT_NAMESPACE_BEGIN

/// Exception to be thrown when an error occurs while reading an input file.
class ReadError : public std::runtime_error {
public:
  ReadError( const std::string& message ) :
    std::runtime_error( message.c_str() ) {}
  virtual ~ReadError() throw() {}
};

/// Exception to be thrown when a read error with knowledge of file structure,
/// including a line and column number is encountered.
class FileReadError : public ReadError {
  std::string mFile;
  int mLine;
  int mColumn;

public:
  FileReadError(
    const std::string& file,
    int line,
    int column,
    const std::string& reason ) :
    ReadError( reason ),
    mFile( file ),
    mLine( line ),
    mColumn( column ) {}
  virtual ~FileReadError() throw() {}
  virtual const char* what() const throw() {
    try {
      static std::string message;
      if ( message.empty() ) {
        std::stringstream ss;
        ss << "Error reading " << mFile << " at line ";
        ss << mLine << ", column" << mColumn;
        ss << ": " << ReadError::what();
        message = ss.str();
      }
      return message.c_str();
    } catch ( ... ) {
      return ReadError::what();
    }
  }

  int line() const { return mLine; }
  int column() const { return mColumn; }
};

/// Interface for types that read items from files. Implement this interface to
/// read a tree of items from a file.
class Reader : public xdm::ReferencedObject {
public:

  /// The result type for reading a file. The first element is the newly
  /// constructed Item, the second element is the number of series indices read.
  typedef std::pair< xdm::RefPtr< xdm::Item >, std::size_t > ReadResult;

  Reader();
  virtual ~Reader();

  /// Read an Item from the file with the given path.
  /// @param path Path to the file to read.
  /// @return A ReadResult with the Item and the number of series elements read.
  /// @throw ReadError There was an error reading from the path.
  /// @throw FileReadError There was an error reading at a line and column in
  ///        the file.
  virtual ReadResult readItem( const xdm::FileSystemPath& path ) = 0;

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
