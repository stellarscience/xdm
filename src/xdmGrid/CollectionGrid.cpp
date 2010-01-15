#include <xdmGrid/CollectionGrid.hpp>

#include <algorithm>

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

void CollectionGrid::traverse( xdm::ItemVisitor& iv ) {
  std::for_each( mChildren.begin(), mChildren.end(), xdm::ApplyVisitor( iv ) );
}

XDM_GRID_NAMESPACE_END

