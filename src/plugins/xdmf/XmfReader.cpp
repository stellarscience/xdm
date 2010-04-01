//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmf/XmfReader.hpp>

#include <xdm/Item.hpp>
#include <xdm/RefPtr.hpp>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <sstream>

XDMF_NAMESPACE_BEGIN

namespace {

//-- Tags in the XDMF Specification --//
char const * const kAttributeTag   = "Attribute";
char const * const kDataItemTag    = "DataItem";
char const * const kDomainTag      = "Domain";
char const * const kGeometryTag    = "Geometry";
char const * const kGridTag        = "Grid";
char const * const kInformationTag = "Information";
char const * const kXdmfTag        = "Xdmf";
char const * const kTopologyTag    = "Topology";
char const * const kTimeTag        = "Time";

// Exception to signal invalid XML (bad document structure).
class XdmfInvalidTagError : public xdmFormat::ReadError {
  std::string mActualTag;
  std::string mExpectedTag;
public:
  XdmfInvalidTagError( 
    const std::string& actualTag, 
    const std::string& expectedTag ) :
    xdmFormat::ReadError( "Invalid XML: " ),
    mActualTag( actualTag ),
    mExpectedTag( expectedTag ) {}
  virtual ~XdmfInvalidTagError() throw() {}
  virtual const char* what() const throw() {
    try {
      std::stringstream ss;
      ss << xdmFormat::ReadError::what();
      ss << "expected " << mExpectedTag;
      ss << ", found" << mActualTag;
      return ss.str().c_str();
    } catch ( ... ) {
      return "Invalid XML";
    }
  }
};

// Build the document given the root node from a parsed and validated XDMF file.
xdm::RefPtr< xdm::Item > buildTree( xmlNode* node ) {
  return xdm::RefPtr< xdm::Item >();
}

} // namespace anon

class XmfReader::Private {
public:
  xmlDocPtr mDocument;

  Private() :
    mDocument( 0 ) {
  }

  ~Private() {
    xmlFreeDoc( mDocument );
  }
};

XmfReader::XmfReader() : 
  xdmFormat::Reader(),
  mImp( new Private ) {
}

XmfReader::~XmfReader() {
}

xdm::RefPtr< xdm::Item > 
XmfReader::readItem( const xdm::FileSystemPath& path ) {
  // Check the file exists.
  if ( !exists( path ) ) {
    XDM_THROW( xdmFormat::ReadError( "Requested path does not exist." ) );
  }
  
  // Parse the document into a tree that can be traversed.
  mImp->mDocument = xmlReadFile( path.pathString().c_str(), NULL, 0 );
  if ( !mImp->mDocument ) {
    // FIXME How do you get the error location from libxml?
    XDM_THROW( xdmFormat::ParseError( 0, 0 ) );
  }

  // Build the tree
  return buildTree( xmlDocGetRootElement( mImp->mDocument ) );
}

bool XmfReader::update( 
  xdm::RefPtr< xdm::Item > item,
  const xdm::FileSystemPath& path,
  std::size_t timeStep ) {
  return false;
}

XDMF_NAMESPACE_END
