//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmFormat_Reader_hpp
#define xdmFormat_Reader_hpp

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

  /// Read an Item from a give file with the given name.
  virtual xdm::RefPtr< xdm::Item > readItem( const std::string& filename ) = 0;

};

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_Reader_hpp
