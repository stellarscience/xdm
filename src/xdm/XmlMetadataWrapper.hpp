//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.      
//                                                                             
// This file is part of XDM                                                    
//                                                                             
// This program is free software: you can redistribute it and/or modify it     
// under the terms of the GNU Lesser General Public License as published by    
// the Free Software Foundation, either version 3 of the License, or (at your  
// option) any later version.                                                  
//                                                                             
// This program is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        
// License for more details.                                                   
//                                                                             
// You should have received a copy of the GNU Lesser General Public License    
// along with this program.  If not, see <http://www.gnu.org/licenses/>.       
//                                                                             
//------------------------------------------------------------------------------
#ifndef xdm_XmlMetadataWrapper_hpp
#define xdm_XmlMetadataWrapper_hpp

#include <xdm/RefPtr.hpp>
#include <xdm/XmlObject.hpp>
#include <xdm/XmlTextContent.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Wrapper to expose only the metadata portion of the XmlObject interface.
/// This allows us to control access to the children of an XmlObject, which we
/// generally only want to expose to certain classes. For instance, when
/// allowing items to specify their XML representation, we want to control how
/// children are added via visitors.  This interface allows us to pass only the
/// portion of the XmlObject required to specify metadata, not aggregate
/// objects.
class XmlMetadataWrapper : public XmlTextContent {
public:
  /// Constructor takes an XmlObject to wrap.
  XmlMetadataWrapper( RefPtr< XmlObject > xml ) : XmlTextContent( xml ) {}
  ~XmlMetadataWrapper() {}

  /// Set the tag for the underlying XmlObject.
  void setTag( const std::string& name ) { mXml->setTag( name ); }
  /// Get the tag for the underlying XmlOBject.
  const std::string& tag() const { return mXml->tag(); }

  /// Set an attribute with the given name to the given value.
  template< typename T >
  void setAttribute( const std::string& key, const T& value ) {
    appendAttribute( *mXml, key, value );
  }
  /// Get a named attribute for the underlying XmlObject.
  const std::string& attribute( const std::string& key ) const {
    return mXml->attribute( key );
  }
};

XDM_NAMESPACE_END

#endif // xdm_XmlMetadataWrapper_hpp

