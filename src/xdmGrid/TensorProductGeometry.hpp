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
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_TensorProductGeometry_hpp

