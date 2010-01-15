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
#ifndef xdm_NamespaceMacro_hpp
#define xdm_NamespaceMacro_hpp

/// Core object model and data interfaces. The xdm namespace contains the
/// definitions of the core xdm data model and interfaces for the data
/// abstraction layer. All other components may use xdm.
namespace xdm {

  /// Implementation details for xdm.
  namespace detail { }

  /// Typedefs for declaring primitive objects in xdm.
  namespace primitiveType { }
}

#ifndef SWIG
# define XDM_NAMESPACE_BEGIN namespace xdm {
#else
# define XDM_NAMESPACE_BEGIN
#endif

#ifndef SWIG
# define XDM_NAMESPACE_END }
#else
# define XDM_NAMESPACE_END
#endif

#endif // xdm_NamespaceMacro_hpp

