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
#ifndef xdmHdf_NamespaceMacro_hpp
#define xdmHdf_NamespaceMacro_hpp

/// Implementations for writing array based datasets using the standard HDF5
/// binary format.  This library contains utilities and implementations of the
/// xdm::Dataset interface for writing data to disk using HDF5 using both the
/// serial and parallel APIs.
namespace xdmHdf { }

#ifndef SWIG
# define XDM_HDF_NAMESPACE_BEGIN namespace xdmHdf {
#else
# define XDM_HDF_NAMESPACE_BEGIN
#endif

#ifndef SWIG
# define XDM_HDF_NAMESPACE_END }
#else
# define XDM_HDF_NAMESPACE_END
#endif

#endif // xdmHdf_NamespaceMacro_hpp

