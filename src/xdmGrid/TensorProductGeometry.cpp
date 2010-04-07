//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//
// This file is part of XDM
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//------------------------------------------------------------------------------
#include <xdmGrid/TensorProductGeometry.hpp>

#include <xdm/VectorRef.hpp>

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
  xdm::RefPtr< xdm::UniformDataItem > data ) {
  assert( dim < dimension() );
  setChild( dim, data );

  // We can automatically set the number of nodes after coordinate values have been specified
  // for all axes.
  int dimensionCount =
    std::count_if( begin(), end(), std::mem_fun_ref( &xdm::RefPtr< xdm::Item >::valid ) );
  if ( dimensionCount == dimension() ) {
    std::size_t nodeProduct = 1;
    for ( ConstIterator childIt = begin(); childIt != end(); ++childIt ) {
      xdm::RefPtr< xdm::UniformDataItem > uniformItem =
        xdm::dynamic_pointer_cast< xdm::UniformDataItem >( *childIt );
      nodeProduct *= uniformItem->typedArray< double >()->size();
    }
    setNumberOfNodes( nodeProduct );
  }
}

void TensorProductGeometry::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Geometry::writeMetadata( xml );

  unsigned int dim = dimension();
  switch ( dim ) {
  case 1:
  case 2:
  case 3:
    xml.setAttribute( "GeometryType", "VxVyVz" );
    break;
  default:
    XDM_THROW( std::domain_error( "Unsupported number of dimensions" ) );
    break;
  }
}

xdm::RefPtr< xdm::VectorRefImp< double > > TensorProductGeometry::createVectorImp()
{
  std::vector< double* > coordinateArrays( dimension() );
  std::vector< size_t > coordinateArraySizes( dimension() );
  for ( size_t i = 0; i < dimension(); i++ ) {
    coordinateArrays[i] = child( i )->typedArray< double >()->begin();
    coordinateArraySizes[i] = child( i )->typedArray< double >()->size();
  }
  return xdm::RefPtr< xdm::VectorRefImp< double > >(
    new xdm::TensorProductArraysImp< double >(
      coordinateArrays, coordinateArraySizes ) );
}

XDM_GRID_NAMESPACE_END

