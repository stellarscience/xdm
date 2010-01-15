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

