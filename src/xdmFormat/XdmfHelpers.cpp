#include <xdmFormat/XdmfHelpers.hpp>

#include <xdm/XmlObject.hpp>

XDM_FORMAT_NAMESPACE_BEGIN

xdm::RefPtr< xdm::XmlObject > createXdmfRoot( const std::string& version ) {
  xdm::RefPtr< xdm::XmlObject > xdmf( new xdm::XmlObject( "Xdmf" ) );
  xdmf->appendAttribute( "Version", version );
  return xdmf;
}

XDM_FORMAT_NAMESPACE_END

