//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmFormat_Writer_hpp
#define xdmFormat_Writer_hpp

#include <xdm/Dataset.hpp>
#include <xdm/FileSystem.hpp>
#include <xdm/Forward.hpp>
#include <xdm/ReferencedObject.hpp>

#include <stdexcept>

#include <xdmFormat/NamespaceMacro.hpp>

XDM_FORMAT_NAMESPACE_BEGIN

/// Base exception for errors that occur while writing data to disk.
class WriteError : public std::runtime_error {
public:
  WriteError( const std::string& message ) :
    std::runtime_error( message.c_str() ) {}
  virtual ~WriteError() throw() {}
};

/// Interface for streaming a series of data to one or more files. Inheritors
/// should implement this interface to stream a data series to a file.
class Writer : public xdm::ReferencedObject {
public:
  Writer();
  virtual ~Writer();

  /// Begin a series of data.
  /// @param path The path to open.
  virtual void open(
    const xdm::FileSystemPath& path,
    xdm::Dataset::InitializeMode mode ) = 0;

  /// Write an Item to the file with the given path.
  /// @param item The tree to write. It is non-const because writing may change the
  ///        state of some of the data items.
  /// @param seriesIndex The index in the series we are writing.
  /// @pre The stream was opened with a call to open.
  /// @throw WriteError Data could not be written to the stream.
  virtual void write( xdm::RefPtr< xdm::Item > item, std::size_t seriesIndex ) = 0;

  /// Close a series that was previously opened, cleaning up any resources that
  /// were required for streaming data.
  virtual void close() = 0;

};

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_Writer_hpp
