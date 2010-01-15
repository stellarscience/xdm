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
#ifndef xdmComm_MpiMessageTag_hpp
#define xdmComm_MpiMessageTag_hpp

#include <xdm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

/// Class containing an enumeration to guarantee unique message tags within the
/// library.  NOTE: These message tags are guaranteed to be unique *within* the
/// library.  In order to ensure that these message tags remain unique within an
/// application, it is necessary for clients to provide communicators that apply
/// only to the functions within the xdmComm library.
class MpiMessageTag {
public:
  enum Value {
    kWriteData,
    kProcessCompleted
  };
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_MpiMessageTag_hpp

