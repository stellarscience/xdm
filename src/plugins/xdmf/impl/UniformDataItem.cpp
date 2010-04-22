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
#include <xdmf/impl/UniformDataItem.hpp>

#include <xdmf/impl/TreeBuilder.hpp>
#include <xdmf/impl/XmlDocumentManager.hpp>
#include <xdmf/impl/XPathQuery.hpp>

#include <xdm/ArrayAdapter.hpp>
#include <xdm/VectorStructuredArray.hpp>

#include <xdmFormat/IoExcept.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <sstream>

namespace xdmf {
namespace impl {

namespace {

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
  return xdm::primitiveType::kFloat;
}

void setContent( UniformDataItem& item, xmlDoc * document, xmlNode * node ) {
  // Get the number type from the NumberType attribute.
  XPathQuery typeQuery( document, node, "@NumberType" );
  std::string typeString;
  if ( typeQuery.size() > 0 ) {
    typeString = typeQuery.textValue( 0 );
  } else {
    typeString = "Float";
  }
  XPathQuery precisionQuery( document, node, "@Precision" );
  size_t precision;
  if ( precisionQuery.size() > 0 ) {
    precision = precisionQuery.getValue( 0, 4 );
  } else {
    precision = 4;
  }
  xdm::primitiveType::Value dataType = type( typeString, precision );
  item.setDataType( dataType );

  // Get the shape from the Dimensions attribute.
  XPathQuery dimensionsQuery( document, node, "@Dimensions" );
  if ( dimensionsQuery.size() == 0 ) {
    XDM_THROW( xdmFormat::ReadError( "No dimensions for a UniformDataItem." ) );
  }
  item.setDataspace( xdm::makeShape( dimensionsQuery.textValue( 0 ) ) );

  // Get the format string for the dataset.
  XPathQuery formatQuery( document, node, "@Format" );
  std::string format( "HDF" );
  if ( formatQuery.size() > 0 ) {
    format = formatQuery.textValue( 0 );
  }

  // Build the dataset.
  if ( format == "HDF" ) {
    xdm::RefPtr< xdmHdf::HdfDataset > itemDataset;
    if ( item.dataset() ) {
      itemDataset = xdm::dynamic_pointer_cast< xdmHdf::HdfDataset >( item.dataset() );
    } else {
      itemDataset = new xdmHdf::HdfDataset;
    }
    XPathQuery datasetInfoQuery( document, node, "text()" );
    if ( datasetInfoQuery.size() == 0 ) {
      XDM_THROW( "No information about requested HDF dataset." );
    }
    std::string datasetInfo = datasetInfoQuery.textValue( 0 );
    std::string fileString;
    xdmHdf::GroupPath groupPath;
    std::string datasetString;
    if ( xdmHdf::parseDatasetInfo( datasetInfo, fileString, groupPath, datasetString ) )
    {
      itemDataset->setFile( fileString );
      itemDataset->setGroupPath( groupPath );
      itemDataset->setDataset( datasetString );
      item.setDataset( itemDataset );
    } else {
      XDM_THROW( xdmFormat::ReadError( "Invalid HDF dataset specification" ) );
    }
  } else {
    XDM_THROW( xdmFormat::ReadError(
      "Only HDF datasets are supported." ) );
  }

  // Give the item an array to hold the data from the dataset should it be
  // loaded into memory.
  xdm::RefPtr< xdm::MemoryAdapter > itemData = item.data();
  if ( !itemData ) {
    xdm::RefPtr< xdm::StructuredArray > array(
      xdm::makeVectorStructuredArray( dataType ) );
    xdm::RefPtr< xdm::ArrayAdapter > adapter( new xdm::ArrayAdapter( array ) );
    adapter->setIsMemoryResident( false );
    item.setData( adapter );
  } else {
    itemData->setNeedsUpdate( true );
  }
}

}

UniformDataItem::UniformDataItem(
  xdm::RefPtr< XmlDocumentManager > doc,
  xdm::RefPtr< SharedNodeVector > stepNodes,
  const std::string& xpathExpr ) :
  xdm::UniformDataItem(),
  Input( doc, stepNodes, xpathExpr ) {
}

UniformDataItem::~UniformDataItem() {
}

void UniformDataItem::read( xmlNode * node, TreeBuilder& builder ) {
  setContent( *this, document()->get(), node );
}

void UniformDataItem::updateState( std::size_t seriesIndex ) {
  setContent( *this, document()->get(), findNode( seriesIndex ) );
}

} // namespace impl
} // namespace xdmf
