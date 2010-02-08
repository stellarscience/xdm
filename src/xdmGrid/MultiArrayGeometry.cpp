//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
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
#include <xdmGrid/NodeRef.hpp>
#include <xdmGrid/MultiArrayGeometry.hpp>

#include <xdm/TypedDataIndexingVisitor.hpp>

#include <algorithm>
#include <cassert>
#include <functional>
#include <stdexcept>
#include <vector>

#include <xdm/ThrowMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

MultiArrayGeometry::MultiArrayGeometry( unsigned int dimension ) :
  Geometry( dimension ) {
  setNumberOfChildren( dimension );
}

MultiArrayGeometry::~MultiArrayGeometry() {
}

void MultiArrayGeometry::setCoordinateValues(
  unsigned int dim,
  xdm::RefPtr< xdm::UniformDataItem > data ) {

  assert( dim < dimension() );
  setChild( dim, data );

  // After all of the coord values are set, then we need to setup the vector
  // shared imp. Since the coord values can be set in any order, we need to
  // first check to see if all dimensions have been assigned coordinates.
  int dimensionCount =
    std::count_if( begin(), end(), std::mem_fun_ref( &xdm::RefPtr< xdm::Item >::valid ) );
  if ( dimensionCount == dimension() ) {

    std::vector< double* > xyzArrays;
    std::vector< std::size_t > axisSizes;
    for ( int dim = 0; dim < dimension(); ++dim ) {
      double* nodeLocationsForOneDim;
      xdm::TypedDataIndexingVisitor< double > visitor( nodeLocationsForOneDim );
      xdm::RefPtr< xdm::UniformDataItem > uniformItem =
        xdm::dynamic_pointer_cast< xdm::UniformDataItem >( child( dim ) );
      uniformItem->accept( visitor );
      xyzArrays.push_back( nodeLocationsForOneDim );
    }

    mSharedVectorImp = new xdm::MultipleArraysOfVectorElementsImpl< double >( xyzArrays );
  }
}

NodeRef MultiArrayGeometry::node( std::size_t nodeIndex )
{
  return NodeRef( xdm::VectorRef< double >( mSharedVectorImp, nodeIndex ) );
}

const NodeRef MultiArrayGeometry::node( std::size_t nodeIndex ) const
{
  return NodeRef( xdm::VectorRef< double >( mSharedVectorImp, nodeIndex ) );
}

void MultiArrayGeometry::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Geometry::writeMetadata( xml );

  unsigned int dim = dimension();
  switch ( dim ) {
  case 1:
    xml.setAttribute( "GeometryType", "X" );
    break;
  case 2:
    xml.setAttribute( "GeometryType", "X_Y" );
    break;
  case 3:
    xml.setAttribute( "GeometryType", "X_Y_Z" );
    break;
  default:
    XDM_THROW( std::domain_error( "Unsupported number of dimensions" ) );
    break;
  }
}

XDM_GRID_NAMESPACE_END

