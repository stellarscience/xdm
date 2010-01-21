//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmFormat_XmfReader_hpp
#define xdmFormat_XmfReader_hpp

#include <xdmFormat/Reader.hpp>

#include <xdmFormat/NamespaceMacro.hpp>

#include <memory>

XDM_FORMAT_NAMESPACE_BEGIN

class XmfReader : public xdmFormat::Reader {
public:
  XmfReader();
  virtual ~XmfReader();

  virtual xdm::RefPtr< xdm::Item > readItem( const std::string& filename );

private:
  // This class uses a private implementation to keep LibXml2 out of the header.
  class Private;
  std::auto_ptr< Private > imp;
};

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_XmfReader_hpp
