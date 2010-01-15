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
#ifndef xdm_UpdateVisitor_hpp
#define xdm_UpdateVisitor_hpp

#include <xdm/ItemVisitor.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Visitor class that traverses the tree invoking update callbacks.  This
/// allows client applications to define application specific update behavior
/// but pass the management of invoking that behavior to the library.
class UpdateVisitor : public xdm::ItemVisitor {
public:

  virtual void apply( UniformDataItem& item );

};

XDM_NAMESPACE_END

#endif // xdm_UpdateVisitor_hpp

