//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmFormat_Reader_hpp
#define xdmFormat_Reader_hpp

#include <xdmFormat/IoExcept.hpp>
#include <xdmFormat/ReadResult.hpp>

#include <xdm/FileSystem.hpp>
#include <xdm/Forward.hpp>
#include <xdm/ReferencedObject.hpp>



namespace xdmFormat {

/// Interface for types that read items from files. Implement this interface to
/// read a tree of items from a file.
class Reader : public xdm::ReferencedObject {
public:

  Reader();
  virtual ~Reader();

  /// Read an Item from the file with the given path.
  /// @param path Path to the file to read.
  /// @return A ReadResult with the Item and the number of series elements read.
  /// @throw ReadError There was an error reading from the path.
  /// @throw FileReadError There was an error reading at a line and column in
  ///        a file.
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

} // namespace xdmFormat

#endif // xdmFormat_Reader_hpp
