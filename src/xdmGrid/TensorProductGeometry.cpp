//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmGrid/TensorProductGeometry.hpp>

#include <stdexcept>

#include <cassert>

#include <xdm/ThrowMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

TensorProductGeometry::TensorProductGeometry( unsigned int dimension ) :
  Geometry( dimension ) {
  setNumberOfChildren( dimension );
}

TensorProductGeometry::~TensorProductGeometry() {
}

void TensorProductGeometry::setCoordinateValues( 
  unsigned int dim,
  xdm::RefPtr< xdm::DataItem > data ) {
  assert( dim < dimension() );
  setChild( dim, data );
}

void TensorProductGeometry::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Geometry::writeMetadata( xml );

  unsigned int dim = dimension();
  switch ( dim ) {
  case 1:
    xml.setAttribute( "GeometryType", "Vx" );
    break;
  case 2:
    xml.setAttribute( "GeometryType", "VxVy" );
    break;
  case 3:
    xml.setAttribute( "GeometryType", "VxVyVz" );
    break;
  default:
    XDM_THROW( std::domain_error( "Unsupported number of dimensions" ) );
    break;
  }
}

XDM_GRID_NAMESPACE_END

