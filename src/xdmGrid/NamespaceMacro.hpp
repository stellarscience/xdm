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
#ifndef xdmGrid_NamespaceMacro_hpp
#define xdmGrid_NamespaceMacro_hpp

/// Components for declaring grid objects. This library contains objects that
/// define structured and unstructured grids and provides the capacity to define
/// attributes over the grids. The grid implementations as well as the
/// implementations for different geometry definitions and multiple structured
/// and unstructured topology definitions are defined here.
namespace xdmGrid { }

#ifndef SWIG
# define XDM_GRID_NAMESPACE_BEGIN namespace xdmGrid {
#else
# define XDM_GRID_NAMESPACE_BEGIN
#endif

#ifndef SWIG
# define XDM_GRID_NAMESPACE_END }
#else
# define XDM_GRID_NAMESPACE_END
#endif

#endif // xdmGrid_NamespaceMacro_hpp

