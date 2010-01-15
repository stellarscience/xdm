#ifndef xdmGrid_GridCompositionMixin_hpp
#define xdmGrid_GridCompositionMixin_hpp

#include <xdmGrid/Grid.hpp>

#include <algorithm>
#include <vector>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Mixin to add the ability for an object to store a collection of grid
/// definitions.  This class encapsulates the functionality so that other
/// classes can reuse it without inheriting from an Item class.
class GridCompositionMixin {
public:
  ~GridCompositionMixin() {}

  /// Append a child grid to the object.
  void appendChildGrid( Grid* g ) {
    mChildren.push_back( g );
  }

  /// Apply a functor to each of the child grids.
  template< typename T > void applyGridFunctor( T& functor ) {
    std::for_each( mChildren.begin(), mChildren.end(), functor );
  }

  /// Apply a const functor to each of the child grids.
  template< typename T > void applyGridFunctor( const T& functor ) {
    std::for_each( mChildren.begin(), mChildren.end(), functor );
  }

protected:
  GridCompositionMixin() {}

private:  
  std::vector< xdm::RefPtr< Grid > > mChildren;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_GridCompositionMixin_hpp

