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
#ifndef xdmGrid_InterlacedGeometry_hpp
#define xdmGrid_InterlacedGeometry_hpp

#include <xdmGrid/Geometry.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Geometric description of the nodes in a mesh represented by an array with
/// interlaced point coordinates.  That is, when the space is of dimension N
/// the coordinates of the mesh nodes at the locations
///
/// (X00, X01, ..., X0N), (X10, X11, ..., X1N), ..., (XM0, XM1, ..., XMN)
///
/// are specified in a single array
///
/// X00 X01 ... X0N X10 X11 ... X1N ... XM0 XM1 ... XMN
///
class InterlacedGeometry : public Geometry {
public:
  InterlacedGeometry( unsigned int dimension );
  virtual ~InterlacedGeometry();

  XDM_META_ITEM( InterlacedGeometry );

  /// Set the DataItem representing the node values.
  void setCoordinateValues( xdm::DataItem* data );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_InterlacedGeometry_hpp

