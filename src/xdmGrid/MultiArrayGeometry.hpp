//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
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
#ifndef xdmGrid_MultiArrayGeometry_hpp
#define xdmGrid_MultiArrayGeometry_hpp

#include <xdmGrid/Geometry.hpp>

#include <xdm/UniformDataItem.hpp>
#include <xdm/VectorRef.hpp>
#include <xdm/RefPtr.hpp>



namespace xdmGrid {

/// Geometric description of the nodes in a mesh represented by a set of
/// arrays each containing point coordinates for one dimension. That is,
/// when the space is of dimension N the coordinates of the mesh nodes
/// at the locations
///
/// (X00, X01, ..., X0N), (X10, X11, ..., X1N), ..., (XM0, XM1, ..., XMN)
///
/// are specified in N arrays
///
/// X00 X10 ... XN0, X01 X11 ... XN1 ..., ..., X0M X1M ... XNM
///
class MultiArrayGeometry : public Geometry {
public:
  MultiArrayGeometry( unsigned int dimension = 0 );
  virtual ~MultiArrayGeometry();

  XDM_META_ITEM( MultiArrayGeometry );

  /// Set the StructuredArray representing the coordinate values in a given
  /// dimension. This is a convenience function that will validate the input
  /// data to ensure that each coordinate axis has the same number of values.
  ///
  /// @param dim The axis of the coordinate represented by the data.
  /// @param data The data representing the coordinate values on the axis.
  /// @throws std::runtime_error if the arrays are not all the same size.
  void setCoordinateValues( unsigned int dim, xdm::RefPtr< xdm::UniformDataItem > data );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

protected:
  virtual void updateDimension();
  virtual xdm::RefPtr< xdm::VectorRefImp< double > > createVectorImp();

};

} // namespace xdmGrid

#endif // xdmGrid_MultiArrayGeometry_hpp

