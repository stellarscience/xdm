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
#include <xdmf/LinearTopologyData.hpp>

#include <xdmGrid/Polyvertex.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataSelection.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/VectorStructuredArray.hpp>

XDMF_NAMESPACE_BEGIN

LinearTopologyData::LinearTopologyData( xdm::RefPtr< xdmGrid::Polyvertex > topology ):
  MemoryAdapter( true ),
  mTopology( topology ),
  mArrayValues()
{
}

LinearTopologyData::~LinearTopologyData()
{
}

xdm::RefPtr< xdm::StructuredArray > LinearTopologyData::array()
{
  if ( !mArrayValues ) {
    mArrayValues = new xdm::VectorStructuredArray< int >;
  }
  if ( mTopology ) {
    if ( mArrayValues->size() != mTopology->numberOfCells() ) {
      size_t oldSize = mArrayValues->size();
      size_t newSize = mTopology->numberOfCells();
      mArrayValues->resize( newSize );
      for ( int i = oldSize; i < newSize; i++ ) {
        (*mArrayValues)[i] = i;
      }
    }
  }
  return mArrayValues;
}

void LinearTopologyData::writeImplementation( xdm::Dataset* dataset ) {
  xdm::RefPtr< xdm::StructuredArray > values = array();
  dataset->serialize( values.get(), xdm::DataSelectionMap() );
}

void LinearTopologyData::readImplementation( xdm::Dataset* dataset ) {
  xdm::RefPtr< xdm::StructuredArray > values = array();
  dataset->deserialize( values.get(), xdm::DataSelectionMap() );
}

xdm::RefPtr< xdm::UniformDataItem > createLinearTopologyUniformDataItem(
  xdm::RefPtr<xdmGrid::Polyvertex>topology )
{
  xdm::RefPtr< xdm::UniformDataItem > result( new xdm::UniformDataItem(
    xdm::PrimitiveTypeInfo< int >::kValue,
    xdm::makeShape( topology->numberOfCells() ) ) );
  result->setData( xdm::makeRefPtr( new LinearTopologyData( topology ) ) );
  return result;
}

XDMF_NAMESPACE_END
