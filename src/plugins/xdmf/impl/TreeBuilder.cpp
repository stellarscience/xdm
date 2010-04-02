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

#include <xdmf/impl/XPathQuery.hpp>

#include <xdm/Item.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdmFormat/Reader.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/CollectionGrid.hpp>
#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/Grid.hpp>
#include <xdmGrid/Topology.hpp>
#include <xdmGrid/UniformGrid.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <sstream>

#include <cassert>

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

//-- XPath queries used to access data in the XML document --//
char const * const kTemporalCollectionGridQuery = 
  "//Grid[@GridType='Collection' and @CollectionType='Temporal']";
char const * const kUniformGridQuery = "//Grid[@GridType='Uniform']";

// Assuming input has been validated, determine type from a string/precision
// pair.
xdm::primitiveType::Value type( const std::string& typeStr, size_t precision ) {
  switch ( precision ) {
  case 1:
    if ( typeStr == "Float" ) return xdm::primitiveType::kFloat;
    if ( typeStr == "Int" ) return xdm::primitiveType::kChar;
    if ( typeStr == "UInt" ) return xdm::primitiveType::kUnsignedChar;
    if ( typeStr == "Char" ) return xdm::primitiveType::kChar;
    break;
  case 4:
    if ( typeStr == "Float" ) return xdm::primitiveType::kFloat;
    if ( typeStr == "Int" ) return xdm::primitiveType::kInt;
    if ( typeStr == "UInt" ) return xdm::primitiveType::kUnsignedInt;
    if ( typeStr == "Char" ) return xdm::primitiveType::kInt;
    break;
  case 8:
    if ( typeStr == "Float" ) return xdm::primitiveType::kDouble;
    if ( typeStr == "Int" ) return xdm::primitiveType::kLongInt;
    if ( typeStr == "UInt" ) return xdm::primitiveType::kLongUnsignedInt;
    if ( typeStr == "Char" ) return xdm::primitiveType::kLongInt;
    break;
  default:
    return xdm::primitiveType::kFloat;
    break;
  }
}

} // namespace

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
TreeBuilder::TreeBuilder( xmlDocPtr document ) :
  mDocument( document ) {
  mXPathContext = xmlXPathNewContext( document );
}

TreeBuilder::~TreeBuilder() {
  xmlXPathFreeContext( mXPathContext );
}

xdm::RefPtr< xdm::Item > TreeBuilder::buildTree() {
  return xdm::RefPtr< xdm::Item >();
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdm::UniformDataItem > 
TreeBuilder::buildUniformDataItem( xmlNode * node ) {
  xdm::RefPtr< xdm::UniformDataItem > result( new xdm::UniformDataItem );

  // Get the number type from the NumberType attribute.
  XPathQuery typeQuery( mXPathContext, node, "@NumberType" );
  std::string typeString;
  if ( typeQuery.size() > 0 ) {
    typeString = typeQuery.textValue( 0 );
  } else {
    typeString = "Float";
  }
  XPathQuery precisionQuery( mXPathContext, node, "@Precision" );
  size_t precision;
  if ( precisionQuery.size() > 0 ) {
    precision = precisionQuery.getValue( 0, 4 );
  } else {
    precision = 4;
  }
  result->setDataType( type( typeString, precision ) );

  // Get the shape from the Dimensions attribute.
  XPathQuery dimensionsQuery( mXPathContext, node, "@Dimensions" );
  if ( dimensionsQuery.size() == 0 ) {
    XDM_THROW( xdmFormat::ReadError( "No dimensions for a UniformDataItem." ) );
  }
  result->setDataspace( xdm::makeShape( dimensionsQuery.textValue( 0 ) ) );

  // Get the format string for the dataset.
  XPathQuery formatQuery( mXPathContext, node, "@Format" );
  std::string format( "HDF" );
  if ( formatQuery.size() == 0 ) {
    format = formatQuery.textValue( 0 );
  }
  
  if ( format == "HDF" ) {
    xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset );
    XPathQuery datasetInfoQuery( mXPathContext, node, "text()" );
    if ( datasetInfoQuery.size() == 0 ) {
      XDM_THROW( "No information about requested HDF dataset." );
    }
    std::string datasetInfo = datasetInfoQuery.textValue( 0 );
    std::string fileString;
    xdmHdf::GroupPath groupPath;
    std::string datasetString;
    if ( xdmHdf::parseDatasetInfo( datasetInfo, fileString, groupPath, datasetString ) )
    {
      dataset->setFile( fileString );
      dataset->setGroupPath( groupPath );
      dataset->setDataset( datasetString );
      result->setDataset( dataset );
    } else {
      XDM_THROW( xdmFormat::ReadError( "Invalid HDF dataset specification" ) );
    }
  } else {
    XDM_THROW( xdmFormat::ReadError(
      "Only HDF datasets are currently supported" ) );
  }

  return result;
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdmGrid::Geometry > TreeBuilder::buildGeometry( xmlNode * node ) {
  return xdm::RefPtr< xdmGrid::Geometry >();
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdmGrid::Topology > TreeBuilder::buildTopology( xmlNode * node ) {
  return xdm::RefPtr< xdmGrid::Topology >();
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdmGrid::Attribute > 
TreeBuilder::buildAttribute( xmlNode * node ) {
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdmGrid::UniformGrid > 
TreeBuilder::buildUniformGrid( xmlNode * node ) {
  return xdm::RefPtr< xdmGrid::UniformGrid >();
}

//------------------------------------------------------------------------------
xdm::RefPtr< xdm::Item > 
TreeBuilder::buildTemporalCollectionGrid( xmlNode * node ) {
  return xdm::RefPtr< xdm::Item >();
}


} // namespace impl
} // namespace xdmf
