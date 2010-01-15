#include <xdmGrid/CollectionGrid.hpp>

XDM_GRID_NAMESPACE_BEGIN

CollectionGrid::CollectionGrid( CollectionType type ) :
  Grid(),
  mType( type ),
  mChildren() {
}

CollectionGrid::~CollectionGrid() {
}

void CollectionGrid::setType( CollectionType t ) {
  mType = t;
}

CollectionGrid::CollectionType CollectionGrid::type() const {
  return mType;
}

void CollectionGrid::appendChild( Grid* g ) {
  mChildren.push_back( g );
}

XDM_GRID_NAMESPACE_END

