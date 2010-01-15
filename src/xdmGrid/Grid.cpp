#include <xdmGrid/Grid.hpp>

XDM_GRID_NAMESPACE_BEGIN

Grid::Grid() :
  xdm::Item() {
}

Grid::~Grid() {
}

void Grid::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Item::writeMetadata( xml );
  xml.setTag( "Grid" );
}

XDM_GRID_NAMESPACE_END

