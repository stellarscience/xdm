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
#include <xdmf/impl/Time.hpp>

#include <xdmf/impl/TreeBuilder.hpp>
#include <xdmf/impl/XmlDocumentManager.hpp>
#include <xdmf/impl/XPathQuery.hpp>

#include <xdm/UniformDataItem.hpp>

#include <xdmFormat/IoExcept.hpp>

#include <sstream>

namespace xdmf {
namespace impl {

Time::Time(
  xdm::RefPtr< XmlDocumentManager > doc,
  xdm::RefPtr< SharedNodeVector > stepNodes,
  const std::string& xpathExpr ) :
  xdmGrid::Time(),
  Input( doc, stepNodes, xpathExpr ) {
}

Time::~Time() {
}

void Time::read( xmlNode * node, TreeBuilder& builder ) {
  XPathQuery typeQuery( document()->get(), node, "@TimeType" );
  std::string timeType = (typeQuery.size() > 0)?typeQuery.textValue(0):"Single";

  if ( timeType == "Single" ) {
    XPathQuery valueQuery( document()->get(), node, "@Value" );
    if ( valueQuery.size() == 0 ) {
      XDM_THROW( xdmFormat::ReadError(
        "Single XDMF grid time specified with no Value" ) );
    }
    std::stringstream ss( valueQuery.textValue( 0 ) );
    double value;
    ss >> value;
    if ( ss.bad() ) {
      XDM_THROW( xdmFormat::ReadError( "Unable to read XDMF Time Value." ) );
    }
    setValue( value );
  } else if ( timeType == "List" ) {
    XPathQuery valuesQuery( document()->get(), node, "DataItem" );
    if ( valuesQuery.size() == 0 ) {
      XDM_THROW( xdmFormat::ReadError( "No values found for XDMF Time." ) );
    }

    // Read the uniform data item containing the values.
    xdm::RefPtr< xdm::UniformDataItem > data =
      builder.buildUniformDataItem( valuesQuery.node( 0 ) );
    xdm::RefPtr< xdm::TypedStructuredArray< double > > array =
      data->typedArray< double >();
    std::vector< double > values( array->size() );
    std::copy( array->begin(), array->end(), values.begin() );
    setValues( values );
  } else {
    XDM_THROW( xdmFormat::ReadError( "Unrecognized XDMF Time type." ) );
  }
}

void Time::updateState( std::size_t seriesIndex ) {
  TreeBuilder builder( document(), nodes() );
  read( findNode( seriesIndex ), builder );
}

} // namespace impl
} // namespace xdmf
