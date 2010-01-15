#include <xdmGrid/CollectionGrid.hpp>

#include <algorithm>
#include <stdexcept>

#include <xdm/ThrowMacro.hpp>

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

void CollectionGrid::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Grid::writeMetadata( xml );

  // write the grid type as a collection
  xml.setAttribute( "GridType", "Collection" );

  // write the type of collection.
  switch ( mType ) {
  case kSpatial:
    xml.setAttribute( "CollectionType", "Spatial" );
    break;
  case kTemporal:
    xml.setAttribute( "CollectionType", "Temporal" );
    break;
  default:
    XDM_THROW( std::invalid_argument( "Unrecognized Grid type" ) );
    break;
  }
}

XDM_GRID_NAMESPACE_END

