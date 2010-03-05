//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmFormat_Reader_hpp
#define xdmFormat_Reader_hpp

#include <xdm/FileSystem.hpp>
#include <xdm/Item.hpp>
#include <xdm/RefPtr.hpp>

#include <stdexcept>
#include <string>

#include <xdmFormat/NamespaceMacro.hpp>

XDM_FORMAT_NAMESPACE_BEGIN

/// Interface for types that read items from files. Implement this interface to
/// read a tree of items from a file.
class Reader {
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
  virtual bool update( xdm::RefPtr< xdm::Item > item ) = 0;

};

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_Reader_hpp
