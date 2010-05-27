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

#include <xdmf/impl/TreeBuilder.hpp>

#include <xdmf/impl/XmlDocumentManager.hpp>
#include <xdmf/impl/XPathQuery.hpp>

#include <xdmf/impl/Time.hpp>
#include <xdmf/impl/UniformDataItem.hpp>

#include <xdm/Algorithm.hpp>
#include <xdm/ArrayAdapter.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/Item.hpp>
#include <xdm/VectorStructuredArray.hpp>

#include <xdmFormat/IoExcept.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/CollectionGrid.hpp>
#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/Grid.hpp>
#include <xdmGrid/InterlacedGeometry.hpp>
#include <xdmGrid/MultiArrayGeometry.hpp>
#include <xdmGrid/RectilinearMesh.hpp>
#include <xdmGrid/TensorProductGeometry.hpp>
#include <xdmGrid/Time.hpp>
#include <xdmGrid/Topology.hpp>
#include <xdmGrid/UniformGrid.hpp>

#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <algorithm>
#include <map>
#include <sstream>
#include <string>

#include <cassert>
#include <cstring>

namespace xdmf {
namespace impl {

namespace {

//-- Tags in the XDMF Specification --//
char const * const kAttributeTag   = "Attribute";
char const * const kDataItemTag    = "DataItem";
char const * const kDomainTag      = "Domain";
char const * const kGeometryTag    = "Geometry";
char const * const kGridTag        = "Grid";
char const * const kInformationTag = "Information";
char const * const kXdmfTag        = "Xdmf";
char const * const kTopologyTag    = "Topology";
char const * const kTimeTag        = "Time";

// XDMF Geometry Types.
enum XdmfGeometryType {
  INTERLACED_3D = 0, // XYZ
  INTERLACED_2D, // XY
  MULTI_ARRAY, // X_Y_Z
  TENSOR_PRODUCT, // VxVyVz
  ORIGIN_OFFSET // Origin_DxDyDz
};

// Generate an XPath expression that references descendant in the context of
// ancestor.
std::string generateXPathExpr(
  xmlDoc * doc,
  xmlNode * descendant,
  xmlNode * ancestor )
{
  // Find the path from the descendant to the ancestor.
  NodePath path = findPathToAncestor( doc, descendant, ancestor );
  // Reverse the result to get the path from the ancestor to the descendant.
  std::reverse( path.begin(), path.end() );
  // Return the query string.
  return makeXPathQuery( path );
}

// Make sure a UniformDataItem read has double precision type.
void forceDouble( xdm::RefPtr< xdm::UniformDataItem > item ) {
  if ( item->dataType() != xdm::primitiveType::kDouble ) {
    item->setDataType( xdm::primitiveType::kDouble );
    xdm::RefPtr< xdm::StructuredArray > array(
      xdm::makeVectorStructuredArray( xdm::primitiveType::kDouble ) );
    xdm::RefPtr< xdm::ArrayAdapter > adapter( new xdm::ArrayAdapter( array ) );
    adapter->setIsMemoryResident( false );
    item->setData( adapter );
  }
}

} // namespace

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

TreeBuilder::TreeBuilder(
  xdm::RefPtr< XmlDocumentManager > doc,
  xdm::RefPtr< SharedNodeVector > seriesGrids ) :
  mDoc( doc ),
  mSeriesGrids( seriesGrids ) {
}

TreeBuilder::~TreeBuilder() {
}

xdm::RefPtr< xdm::Item > TreeBuilder::buildTree() {
  if ( mSeriesGrids->size() < 1 ) {
    XDM_THROW( xdmFormat::ReadError( "XDMF file contains no data.") );
  }
  return buildGrid( mSeriesGrids->at( 0 ) );
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdm::UniformDataItem >
TreeBuilder::buildUniformDataItem( xmlNode * node ) {
  std::string path = generateXPathExpr( mDoc->get(), node, mSeriesGrids->at( 0 ) );
  // Build the implementation item and return it.
  xdm::RefPtr< UniformDataItem > result( new UniformDataItem(
    mDoc,
    mSeriesGrids,
    path ) );
  result->read( node, *this );
  readItem( result, node );
  return result;
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdmGrid::Geometry > TreeBuilder::buildGeometry( xmlNode * node ) {

  XdmfGeometryType geometryType( INTERLACED_3D );

  // Mapping from 'GeometryType' attribute value to geometry type enum.
  typedef std::map< std::string, XdmfGeometryType > TypeMap;
  static TypeMap typeMap;
  if ( typeMap.empty() ) {
    typeMap["XYZ"] = INTERLACED_3D;
    typeMap["XY"] = INTERLACED_2D;
    typeMap["X_Y_Z"] = MULTI_ARRAY;
    typeMap["VXVYVZ"] = TENSOR_PRODUCT;
    typeMap["ORIGIN_DXDYDZ"] = ORIGIN_OFFSET;
  }

  // Get the geometry type from the GeometryType attribute.
  XPathQuery geometryTypeQuery( mDoc->get(), node, "@GeometryType" );
  if ( geometryTypeQuery.size() > 0 ) {
    std::string typeString = geometryTypeQuery.textValue( 0 );
    xdm::toUpper( typeString );
    TypeMap::const_iterator type = typeMap.find( typeString );
    if ( type != typeMap.end() ) {
      geometryType = type->second;
    } else {
      XDM_THROW( xdmFormat::ReadError( "Unrecognized XDMF GeometryType." ) );
    }
  }

  // Read the internal data items that make up the values for the geometry.
  XPathQuery dataQuery( mDoc->get(), node, kDataItemTag );
  if ( dataQuery.size() == 0 ) {
    XDM_THROW( xdmFormat::ReadError( "XDMF Geometry values unspecified." ) );
  }
  size_t dataItemCount = dataQuery.size();

  // Build the specific Geometry subclass and assign the coordinate values
  // accordingly.  All coordinate values are turned into doubles as we do
  // this. This is because XDM assumes we use double precision floating point
  // values for Geometry coordinate value specifications. It is enough to set
  // this here because the HDF library will convert from the type that is
  // actually in the dataset referenced by the uniform DataItem when the data
  // is loaded later.
  xdm::RefPtr< xdmGrid::Geometry > result;
  if ( geometryType == INTERLACED_3D ) {
    xdm::RefPtr< xdmGrid::InterlacedGeometry > g(
      new xdmGrid::InterlacedGeometry( 3 ) );
    xdm::RefPtr< xdm::UniformDataItem > data = buildUniformDataItem( dataQuery.node( 0 ) );
    forceDouble( data );
    g->setCoordinateValues( data );
    result = g;
  } else if ( geometryType == INTERLACED_2D ) {
    xdm::RefPtr< xdmGrid::InterlacedGeometry > g(
      new xdmGrid::InterlacedGeometry( 2 ) );
    xdm::RefPtr< xdm::UniformDataItem > data = buildUniformDataItem( dataQuery.node( 0 ) );
    forceDouble( data );
    g->setCoordinateValues( data );
    result = g;
  } else if ( geometryType == MULTI_ARRAY ) {
    xdm::RefPtr< xdmGrid::MultiArrayGeometry > g(
      new xdmGrid::MultiArrayGeometry );
    // dimension is implicit from the number of DataItems under the Geometry
    g->setDimension( dataItemCount );
    for ( size_t i = 0; i < dataItemCount; ++i ) {
      xdm::RefPtr< xdm::UniformDataItem > data = buildUniformDataItem( dataQuery.node( i ) );
      forceDouble( data );
      g->setCoordinateValues( i, data );
    }
    result = g;
  } else if ( geometryType == TENSOR_PRODUCT ) {
    xdm::RefPtr< xdmGrid::TensorProductGeometry > g(
      new xdmGrid::TensorProductGeometry );
    // dimension is implicit from the number of DataItems under the Geometry
    g->setDimension( dataItemCount );
    for ( size_t i = 0; i < dataItemCount; ++i ) {
      xdm::RefPtr< xdm::UniformDataItem > data = buildUniformDataItem( dataQuery.node( i ) );
      forceDouble( data );
      g->setCoordinateValues( i, data );
    }
    result = g;
  } else if ( geometryType == ORIGIN_OFFSET ) {
    XDM_THROW( xdmFormat::ReadError( "Origin offset geometry not yet supported." ) );
  } else {
    XDM_THROW( xdmFormat::ReadError( "Unregognized XDMF GeometryType." ) );
  }

  readItem( result, node );
  return result;
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdmGrid::Topology > TreeBuilder::buildTopology( xmlNode * node ) {

  enum {
    POLYVERTEX = 0,
    POLYLINE,
    POLYGON,
    TRIANGLE,
    QUADRILATERAL,
    TETRAHEDRON,
    PYRAMID,
    WEDGE,
    HEXAHEDRON,
    EDGE_3,
    TRIANGLE_6,
    QUADRILATERAL_8,
    TETRAHEDRON_10,
    PYRAMID_13,
    WEDGE_15,
    HEXAHEDRON_20,
    MIXED,
    SMESH2D,
    RECTMESH2D,
    CORECTMESH2D,
    SMESH3D,
    RECTMESH3D,
    CORECTMESH3D
  } topologyType;

  static const char * strings[] = {
    "POLYVERTEX",
    "POLYLINE",
    "POLYGON",
    "TRIANGLE",
    "QUADRILATERAL",
    "TETRAHEDRON",
    "PYRAMID",
    "WEDGE",
    "HEXAHEDRON",
    "EDGE_3",
    "TRIANGLE_6",
    "QUADRILATERAL_8",
    "TETRAHEDRON_10",
    "PYRAMID_13",
    "WEDGE_15",
    "HEXAHEDRON_20",
    "MIXED",
    "2DSMESH",
    "2DRECTMESH",
    "2DCORECTMESH",
    "3DSMESH",
    "3DRECTMESH",
    "3DCORECTMESH"
  };

  // Read the topology type attribute.
  XPathQuery topologyTypeQuery( mDoc->get(), node, "@TopologyType" );
  if ( topologyTypeQuery.size() == 0 ) {
    XDM_THROW( xdmFormat::ReadError( "No XDMF topology type specified." ) );
  }
  std::string typeString = topologyTypeQuery.textValue( 0 );
  xdm::toUpper( typeString );
  if ( typeString == strings[POLYVERTEX] ) topologyType = POLYVERTEX;
  else if ( typeString == strings[POLYLINE] ) topologyType = POLYLINE;
  else if ( typeString == strings[POLYGON] ) topologyType = POLYGON;
  else if ( typeString == strings[TRIANGLE] ) topologyType = TRIANGLE;
  else if ( typeString == strings[QUADRILATERAL] ) topologyType = QUADRILATERAL;
  else if ( typeString == strings[TETRAHEDRON] ) topologyType = TETRAHEDRON;
  else if ( typeString == strings[PYRAMID] ) topologyType = PYRAMID;
  else if ( typeString == strings[WEDGE] ) topologyType = WEDGE;
  else if ( typeString == strings[HEXAHEDRON] ) topologyType = HEXAHEDRON;
  else if ( typeString == strings[EDGE_3] ) topologyType = EDGE_3;
  else if ( typeString == strings[TRIANGLE_6] ) topologyType = TRIANGLE_6;
  else if ( typeString == strings[QUADRILATERAL_8] ) topologyType = QUADRILATERAL_8;
  else if ( typeString == strings[TETRAHEDRON_10] ) topologyType = TETRAHEDRON_10;
  else if ( typeString == strings[PYRAMID_13] ) topologyType = PYRAMID_13;
  else if ( typeString == strings[WEDGE_15] ) topologyType = WEDGE_15;
  else if ( typeString == strings[HEXAHEDRON_20] ) topologyType = HEXAHEDRON_20;
  else if ( typeString == strings[MIXED] ) topologyType = MIXED;
  else if ( typeString == strings[SMESH2D] ) topologyType = SMESH2D;
  else if ( typeString == strings[RECTMESH2D] ) topologyType = RECTMESH2D;
  else if ( typeString == strings[CORECTMESH2D] ) topologyType = CORECTMESH2D;
  else if ( typeString == strings[SMESH3D] ) topologyType = SMESH3D;
  else if ( typeString == strings[RECTMESH3D] ) topologyType = RECTMESH3D;
  else if ( typeString == strings[CORECTMESH3D] ) topologyType = CORECTMESH3D;
  else XDM_THROW( xdmFormat::ReadError( "Unrecognized XDMF topology type" ) );

  xdm::RefPtr< xdmGrid::Topology > result;
  if ( topologyType <= MIXED ) {
    // Unstructured topologies.
    XDM_THROW( xdmFormat::ReadError( "Unstructured XDMF topologies are unsupported." ) );
  } else {
    xdm::RefPtr< xdmGrid::RectilinearMesh > structuredTopology(
      new xdmGrid::RectilinearMesh );
      // Get the attribute that specifies the shape of the structured topology
    XPathQuery shapeQuery( mDoc->get(), node, "@Dimensions|@NumberOfElements" );
    if ( shapeQuery.size() > 0 ) {
      xdm::DataShape<> topologyShape = xdm::makeShape(shapeQuery.textValue(0));
      // XDMF Specifies topology in as node dimensions in ZYX order. The XDM
      // library convention is cell dimensions in XYZ order. So we reverse the
      // order and subtract 1 from each dimension.
      topologyShape.reverseDimensionOrder();
      for ( 
        xdm::DataShape<>::DimensionIterator dim = topologyShape.begin(); 
        dim != topologyShape.end();
        ++dim ) {
        *dim -= 1;
      }
      structuredTopology->setShape( topologyShape );
    } else {
      XDM_THROW( xdmFormat::ReadError(
        "No dimensions specified for XDMF structured topology" ) );
    }
    result = structuredTopology;
  }

  // Attach connectivity information specified in DataItem children.
  XPathQuery connectivityQuery( mDoc->get(), node, "DataItem" );
  for ( size_t i = 0; i < connectivityQuery.size(); i++ ) {
    result->appendChild( buildUniformDataItem( connectivityQuery.node( i ) ) );
  }

  readItem( result, node );
  return result;
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdmGrid::Attribute >
TreeBuilder::buildAttribute( xmlNode * node ) {
  // Mapping from XDMF XML attribute type values to enumeration of types.
  typedef std::map< std::string, xdmGrid::Attribute::Type > TypeMap;
  static TypeMap typeMap;
  if ( typeMap.empty() ) {
    typeMap["Scalar"] = xdmGrid::Attribute::kScalar;
    typeMap["Vector"] = xdmGrid::Attribute::kVector;
    typeMap["Tensor"] = xdmGrid::Attribute::kTensor;
    typeMap["Tensor6"] = xdmGrid::Attribute::kTensor6;
    typeMap["Matrix"] = xdmGrid::Attribute::kMatrix;
    typeMap["GlobalID"] = xdmGrid::Attribute::kGlobalId;
  }

  // Mapping from XDMF XML attribute center values to enumeration of centering.
  typedef std::map< std::string, xdmGrid::Attribute::Center > CenterMap;
  static CenterMap centerMap;
  if ( centerMap.empty() ) {
    centerMap["Node"] = xdmGrid::Attribute::kNode;
    centerMap["Cell"] = xdmGrid::Attribute::kElement;
    centerMap["Grid"] = xdmGrid::Attribute::kGrid;
    centerMap["Face"] = xdmGrid::Attribute::kFace;
    centerMap["Edge"] = xdmGrid::Attribute::kEdge;
  }

  // create the result attribute.
  xdm::RefPtr< xdmGrid::Attribute > result( new xdmGrid::Attribute );

  // Get the name of the attribute.
  XPathQuery nameQuery( mDoc->get(), node, "@Name" );
  if ( nameQuery.size() > 0 ) {
    result->setName( nameQuery.textValue( 0 ) );
  }

  // Get the attribute type
  XPathQuery typeQuery( mDoc->get(), node, "@AttributeType" );
  if ( typeQuery.size() > 0 ) {
    TypeMap::const_iterator type = typeMap.find( typeQuery.textValue( 0 ) );
    if ( type != typeMap.end() ) {
      result->setDataType( type->second );
    } else {
      XDM_THROW( xdmFormat::ReadError( "Unrecognized XDMF attribute type" ) );
    }
  } else {
    // default is scalar
    result->setDataType( xdmGrid::Attribute::kScalar );
  }

  // Get the attribute center
  XPathQuery centerQuery( mDoc->get(), node, "@Center" );
  if ( centerQuery.size() > 0 ) {
    CenterMap::const_iterator center = centerMap.find( centerQuery.textValue( 0 ) );
    if ( center != centerMap.end() ) {
      result->setCentering( center->second );
    } else {
      XDM_THROW( xdmFormat::ReadError( "Unrecognized XDMF attribute center." ) );
    }
  } else {
    // default is node centered.
    result->setCentering( xdmGrid::Attribute::kNode );
  }

  // Create the attribute data items.
  XPathQuery dataQuery( mDoc->get(), node, "DataItem" );
  if ( dataQuery.size() == 0 ) {
    XDM_THROW( xdmFormat::ReadError( "XDMF Attribute contains no data." ) );
  }
  result->setDataItem( buildUniformDataItem( dataQuery.node( 0 ) ) );

  readItem( result, node );
  return result;
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdmGrid::UniformGrid >
TreeBuilder::buildUniformGrid( xmlNode * node ) {

  xdm::RefPtr< xdmGrid::UniformGrid > result( new xdmGrid::UniformGrid );

  // Get the topology.
  XPathQuery topologyQuery( mDoc->get(), node, "Topology" );
  if ( topologyQuery.size() == 0 ) {
    XDM_THROW( xdmFormat::ReadError( "XDMF Grid contains no Topology" ) );
  }
  if ( topologyQuery.size() > 1 ) {
    XDM_THROW( xdmFormat::ReadError( "XDMF Grid contains more than one Topology" ) );
  }
  result->setTopology( buildTopology( topologyQuery.node( 0 ) ) );

  // Get the geometry.
  XPathQuery geometryQuery( mDoc->get(), node, "Geometry" );
  if ( geometryQuery.size() == 0 ) {
    XDM_THROW( xdmFormat::ReadError( "XDMF Grid contains no geometry." ) );
  }
  if ( geometryQuery.size() > 1 ) {
    XDM_THROW( xdmFormat::ReadError( "XDMF Grid contains more than one Geometry" ) );
  }
  result->setGeometry( buildGeometry( geometryQuery.node( 0 ) ) );

  // Read all the attributes.
  XPathQuery attributeQuery( mDoc->get(), node, "Attribute" );
  for ( size_t i = 0; i < attributeQuery.size(); i++ ) {
    result->addAttribute( buildAttribute( attributeQuery.node( i ) ) );
  }

  readItem( result, node );
  return result;
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdmGrid::Grid > TreeBuilder::buildGrid( xmlNode * node ) {
  xdm::RefPtr< xdmGrid::Grid > result;
  // Determine the grid type.
  XPathQuery typeQuery( mDoc->get(), node, "@GridType" );
  if ( typeQuery.size() > 0 ) {
    std::string gridType = typeQuery.textValue( 0 );
    if ( gridType == "Uniform" ) {
      result = buildUniformGrid( node );
    } else if ( gridType == "Collection" ) {
      result = buildSpatialCollectionGrid( node );
    } else {
      XDM_THROW( xdmFormat::ReadError( "Unsupported XDMF Grid type" ) );
    }
  } else {
    // UniformGrid is default
    result = buildUniformGrid( node );
  }

  // Check for a Time child for the grid.
  XPathQuery timeQuery( mDoc->get(), node, "Time" );
  if ( timeQuery.size() > 0 ) {
    result->setTime( buildTime( timeQuery.node( 0 ) ) );
  }

  readItem( result, node );
  return result;
}

// -----------------------------------------------------------------------------
xdm::RefPtr< xdmGrid::CollectionGrid >
TreeBuilder::buildSpatialCollectionGrid( xmlNode * node ) {

  xdm::RefPtr< xdmGrid::CollectionGrid > result( new xdmGrid::CollectionGrid );
  // Find all grid children of the node.
  XPathQuery childGridQuery( mDoc->get(), node, "Grid" );
  for ( size_t i = 0; i < childGridQuery.size(); i++ ) {
    result->appendGrid( buildGrid( childGridQuery.node( i ) ) );
  }

  readItem( result, node );
  return result;
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdmGrid::Time > TreeBuilder::buildTime( xmlNode * node ) {
  std::string path = generateXPathExpr( mDoc->get(), node, mSeriesGrids->at( 0 ) );
  xdm::RefPtr< Time > result( new Time( mDoc, mSeriesGrids, path ) );
  readItem( result, node );
  return result;
}

//------------------------------------------------------------------------------
void TreeBuilder::readItem( xdm::RefPtr< xdm::Item > item, xmlNode * node ) {
  XPathQuery nameQuery( mDoc->get(), node, "@Name" );
  if ( nameQuery.size() > 0 ) {
    item->setName( nameQuery.textValue( 0 ) );
  }
}

} // namespace impl
} // namespace xdmf

