//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmFormat/XmfReader.hpp>

#include <libxml/parser.h>
#include <libxml/tree.h>

XDM_FORMAT_NAMESPACE_BEGIN

class XmfReader::Private {
public:
  xmlDocPtr document;

  Private() :
    document( 0 ) {
  }

  ~Private() {
    xmlFreeDoc( document );
  }
};

XmfReader::XmfReader() {
}

XmfReader::~XmfReader() {
}

xdm::RefPtr< xdm::Item > readItem( const std::string& filename ) {
  return xdm::RefPtr< xdm::Item >();
}

XDM_FORMAT_NAMESPACE_END
