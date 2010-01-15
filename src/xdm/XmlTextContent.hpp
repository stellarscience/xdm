//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.      
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
#ifndef xdm_XmlTextContent_hpp
#define xdm_XmlTextContent_hpp

#include <xdm/RefPtr.hpp>
#include <xdm/XmlObject.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Wrapper object that exposes only the text content of an XmlObject.
class XmlTextContent {
protected:
  RefPtr< XmlObject > mXml;

public:
  /// Constructor takes an XmlObject to wrap.
  XmlTextContent( RefPtr< XmlObject > xml ) : mXml( xml ) {}
  ~XmlTextContent() {}

  /// Append a line of content to the XmlObject.
  void appendContentLine( const std::string& line ) { 
    mXml->appendContent( line );
  }

  /// Query a line of content from the XmlObject.
  const std::string& contentLine( unsigned int line ) const {
    return mXml->contentLine( line );
  }
};

XDM_NAMESPACE_END

#endif // xdm_XmlTextContent_hpp

