#include <xdmGrid/Grid.hpp>
#include <xdmGrid/Time.hpp>

XDM_GRID_NAMESPACE_BEGIN

Grid::Grid() :
  xdm::Item() {
}

Grid::~Grid() {
}

Time* Grid::time() {
  return mTime.get();
}

const Time* Grid::time() const {
  return mTime.get();
}

void Grid::setTime( Time* time ) {
  mTime = time;
}

void Grid::traverse( xdm::ItemVisitor& iv ) {
  if ( mTime.valid() ) {
    mTime->accept( iv );
  }
}

void Grid::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Item::writeMetadata( xml );
  xml.setTag( "Grid" );
}

XDM_GRID_NAMESPACE_END

