//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmf_XmfReader_hpp
#define xdmf_XmfReader_hpp

#include <xdmFormat/Reader.hpp>

#include <xdmf/NamespaceMacro.hpp>

#include <memory>
#include <sstream>

XDMF_NAMESPACE_BEGIN

class ValidationError : public xdmFormat::ReadError {
  std::string mFile;
  int mLine;
  int mColumn;
  std::string mReason;
public:
  ValidationError( const std::string& file, int line, int column, const std::string& reason ) :
    xdmFormat::ReadError( reason ),
    mFile( file ),
    mLine( line ),
    mColumn( column ),
    mReason( reason ) {}
  virtual ~ValidationError() throw() {}

  virtual const char * what() const throw() {
    try {
      static std::string result;
      std::stringstream msg;
      msg << mFile;
      msg << " validation error at line " << mLine << " column " << mColumn;
      msg << ": " << mReason;
      result = msg.str();
      return result.c_str();
    } catch( ... ) {
      return xdmFormat::ReadError::what();
    }
  }
};

class XmfReader : public xdmFormat::Reader {
public:
  XmfReader();
  virtual ~XmfReader();

  virtual xdm::RefPtr< xdm::Item > readItem( 
    const xdm::FileSystemPath& path );

  virtual bool update(
    xdm::RefPtr< xdm::Item > item,
    const xdm::FileSystemPath& path,
    std::size_t timeStep = 0 );

private:
  // This class uses a private implementation to keep LibXml2 out of the
  // header.
  class Private;
  std::auto_ptr< Private > mImp;
};

XDMF_NAMESPACE_END

#endif // xdmf_XmfReader_hpp
