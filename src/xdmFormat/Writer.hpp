//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmFormat_Writer_hpp
#define xdmFormat_Writer_hpp

#include <xdm/FileSystem.hpp>
#include <xdm/Forward.hpp>

#include <xdmFormat/NamespaceMacro.hpp>

XDM_FORMAT_NAMESPACE_BEGIN

/// Interface for types that write items to files. Implement this interface to
/// write a tree of items to a file.
class Writer {
public:
  Writer();
  virtual ~Writer();

  /// Write an Item to the file with the given path.
  /// @param path Path to the file to write.
  /// @param item The tree to write. It is non-const because writing may change the
  ///        state of some of the data items.
  virtual void writeItem( xdm::RefPtr< xdm::Item > item, const xdm::FileSystemPath& path ) = 0;

  /// Write an existing Item and it's subtree at a new time step. If there is no dynamic
  /// data in the tree, then this function does nothing.
  /// @pre The item was written eariler with writeItem.
  /// @post The data in the output file is updated to reflect the new time step.
  /// @param item An existing item tree.
  /// @return True if new data was written, false otherwise.
  virtual bool update(
    xdm::RefPtr< xdm::Item > item,
    const xdm::FileSystemPath& path,
    std::size_t timeStep = 0 ) = 0;

};

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_Writer_hpp
