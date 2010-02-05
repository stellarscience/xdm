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
#include <xdmGrid/CellRef.hpp>
#include <xdmGrid/UnstructuredTopology.hpp>

#include <xdm/TypedDataIndexingVisitor.hpp>

#include <sstream>

XDM_GRID_NAMESPACE_BEGIN

UnstructuredTopology::UnstructuredTopology() :
  Topology(),
  mCellSharedImp(),
  mConnectivity(),
  mCellType( CellType::Default ),
  mNumberOfCells(),
  mOrdering() {
}

UnstructuredTopology::~UnstructuredTopology() {
}

void UnstructuredTopology::setNumberOfCells( std::size_t numberOfCells ) {
  mNumberOfCells = numberOfCells;
}

std::size_t UnstructuredTopology::numberOfCells() const {
  return mNumberOfCells;
}

void UnstructuredTopology::setCellType( const CellType::Type& type ) {
  mCellType = type;
}

const CellType::Type& UnstructuredTopology::cellType() const {
  return mCellType;
}

void UnstructuredTopology::setNodeOrdering( const NodeOrderingConvention::Type& order )
{
  mOrdering = order;
}

NodeOrderingConvention::Type UnstructuredTopology::nodeOrdering() const
{
  return mOrdering;
}

CellRef UnstructuredTopology::cell( std::size_t cellIndex )
{
  return CellRef( mCellSharedImp, cellIndex );
}

const CellRef UnstructuredTopology::cell( std::size_t cellIndex ) const
{
  return CellRef( mCellSharedImp, cellIndex );
}

void UnstructuredTopology::setConnectivity( xdm::RefPtr< xdm::UniformDataItem > connectivity ) {
  mConnectivity = connectivity;
  std::size_t* connectivityArray;
  xdm::TypedDataIndexingVisitor< std::size_t > visitor( connectivityArray );
//  mCellSharedImp = new xdm::SingleArrayOfVectorsImpl< double >(
}

void UnstructuredTopology::traverse( xdm::ItemVisitor& iv ) {
  if ( mConnectivity.valid() ) {
    mConnectivity->accept( iv );
  }
}

void UnstructuredTopology::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Topology::writeMetadata( xml );

  // Write the number of cells
  xml.setAttribute( "NumberOfElements", mNumberOfCells );

  // Write the cell type
  xml.setAttribute( "ElementType", mCellType.shapeName() );

  // Write the nodes per cell
  xml.setAttribute( "NodesPerElement", mCellType.nodesPerCell() );
}

XDM_GRID_NAMESPACE_END
