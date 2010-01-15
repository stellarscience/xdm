//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmGrid_TensorProductGeometry_hpp
#define xdmGrid_TensorProductGeometry_hpp

#include <xdmGrid/Geometry.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Geometric description of geometry values by taking the tensor product of
/// n arrays, where n is the dimension of the space.  That is, given n sets of
/// values
///
/// S_0 = {a_0, ..., a_l}, S_1 = {b_0, ..., b_m}, ..., S_N
///
/// The geometry will be set of all ordered tuples (a, b, ...) where the first
/// coordinate is taken from S_0, the second from S_1, etc.  The resulting size
/// of the geometry will then be |S_0|*|S_1|*...*|S_N|.
class TensorProductGeometry : public Geometry {
public:
  TensorProductGeometry( unsigned int dimension );
  virtual ~TensorProductGeometry();

  XDM_META_ITEM( TensorProductGeometry );

  /// Set the StructuredArray representing the coordinate values in a given 
  /// dimension. This is a convenience function that will validate the input 
  /// data to ensure that each coordinate axis has the same number of values.
  ///
  /// @param dim The axis of the coordinate represented by the data.
  /// @param data The data representing the coordinate values on the axis.
  void setCoordinateValues( unsigned int dim, xdm::RefPtr< xdm::DataItem > data );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_TensorProductGeometry_hpp

