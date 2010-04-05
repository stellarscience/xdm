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
#include <xdmIntegrationTest/FunctionData.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/HyperSlabBlockIterator.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/TypedStructuredArray.hpp>
#include <xdm/ThrowMacro.hpp>
#include <xdm/VectorStructuredArray.hpp>
#include <xdm/ArrayAdapter.hpp>

#include <xdmGrid/UniformGrid.hpp>
#include <xdmGrid/RectilinearMesh.hpp>
#include <xdmGrid/TensorProductGeometry.hpp>
#include <xdmGrid/Time.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <numeric>
#include <stdexcept>

#include <cmath>

ProblemInfo
constructFunctionGrid( const GridBounds& bounds, const std::string& hdfFile ) {
  const char* axisNames[] = { "X", "Y", "Z" };

  // Grid
  xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid );
  grid->setName( "ProblemGrid" );

  // Time for grid
  xdm::RefPtr< xdmGrid::Time > time( new xdmGrid::Time );
  time->setValue( 0.0 );
  grid->setTime( time );

  // Topology
  xdm::RefPtr< xdmGrid::RectilinearMesh > topology( new xdmGrid::RectilinearMesh );
  topology->setShape( xdm::makeShape( 
    bounds.size(2) + 1, 
    bounds.size(1) + 1,
    bounds.size(0) + 1 ) );
  grid->setTopology( topology );

  // Geometry
  xdm::RefPtr< xdmGrid::TensorProductGeometry > geometry( 
    new xdmGrid::TensorProductGeometry( 3 ) );
  grid->setGeometry( geometry );

  // assign geometry values
  for ( int i = 0; i < 3 ; i++ ) {
    xdm::RefPtr< xdm::UniformDataItem > dataItem( new xdm::UniformDataItem( 
      xdm::primitiveType::kDouble, xdm::makeShape( bounds.size(i) + 1 ) ) );
    // allocate enough space for bounds + 1 vertices to encompass the whole
    // range.
    xdm::RefPtr< xdm::VectorStructuredArray< double > > values( 
      new xdm::VectorStructuredArray< double >( bounds.size(i) + 1 ) );
    for ( int j = 0; j < bounds.size(i); j++ ) {
      (*values)[j] = bounds.nodeCoordinate( i, j );
    }
    // fill the final point
    (*values)[ bounds.size(i) ] = bounds.bounds(i).second;
    dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( values ) ) );
    geometry->setCoordinateValues( i, dataItem );
    xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset );
    dataset->setFile( hdfFile.c_str() );
    dataset->setDataset( axisNames[i] );
    dataItem->setDataset( dataset );
  }

  // construct an attribute to hold the function values
  xdm::RefPtr< xdmGrid::Attribute > attribute = xdmGrid::createAttribute (
    grid, 
    xdmGrid::Attribute::kCell, 
    xdmGrid::Attribute::kScalar, 
    "FunctionValues",
    xdm::primitiveType::kDouble );
  grid->appendChild( attribute );
  xdm::RefPtr< xdmHdf::HdfDataset > attributeDataset( new xdmHdf::HdfDataset );
  attributeDataset->setFile( hdfFile.c_str() );
  attributeDataset->setDataset( "FunctionValues" );
  // use chunked IO and compression because we can.
  attributeDataset->setUseChunkedIo( true );
  attributeDataset->setUseCompression( true );
  attribute->dataItem()->setDataset( attributeDataset );
  
  return std::make_pair( grid.get(), attribute.get() );
}

//-----------------------------------------------------------------------------
FunctionData::FunctionData( 
  const GridBounds& grid, 
  const xdm::HyperSlab<>& region,
  xdm::RefPtr< Function > function,
  const xdm::DataShape<>& blockSize ) :
  mGrid( grid ),
  mRegionOfInterest( region ),
  mStorage(),
  mStructuredArray(),
  mFunction( function ),
  mBlockSize( blockSize ) {
  
  using std::accumulate;
  typedef std::multiplies< std::vector< double >::size_type > MultiplySize;

  mStorage.resize( 
    accumulate( 
      mBlockSize.begin(), 
      mBlockSize.end(),
      1,
      MultiplySize() ) );
  mStructuredArray = new xdm::TypedStructuredArray< double >(
    &mStorage[0], mStorage.size() );
}

FunctionData::~FunctionData() {
}

xdm::RefPtr< xdm::StructuredArray > FunctionData::array() {
  XDM_THROW( std::runtime_error( "Random access not implemented" ) );
}

void FunctionData::writeImplementation( xdm::Dataset* dataset ) {
  using std::accumulate;
  typedef std::multiplies< xdm::DataShape<>::size_type > MultiplySize;

  xdm::HyperSlabBlockIterator<> block( mRegionOfInterest, mBlockSize );
  xdm::HyperSlabBlockIterator<> end;
  for( /* no-init */; block != end; ++block ) {
    int startI = block->start(0);
    int startJ = block->start(1);
    int startK = block->start(2);
    int sizeI = block->count(0);
    int sizeJ = block->count(1);
    int sizeK = block->count(2);

    for ( int k = 0; k < sizeK; k++ ) {
      double z = mGrid.cellCoordinate( 2, startK + k );
      for ( int j = 0; j < sizeJ; j++ ) {
        double y = mGrid.cellCoordinate( 1, startJ + j );
        for ( int i = 0; i < sizeI; i++ ) {
          double x = mGrid.cellCoordinate( 0, startI + i );
          double result = (*mFunction)( x, y, z );
          int arrayLocation = i + j*sizeI + k*sizeI*sizeJ;
          mStorage[arrayLocation] = result;
        }
      }
    }

    xdm::DataShape<>::size_type blockSize = accumulate(
      block->beginCount(), block->endCount(), 1, MultiplySize() );
    mStructuredArray->setSize( blockSize );

    xdm::HyperSlab<> fileSelectionSlab( *block );
    reverseDimensionOrder( fileSelectionSlab );
    xdm::DataSelectionMap selectionMap(
      xdm::makeRefPtr( new xdm::AllDataSelection ),
      xdm::makeRefPtr( new xdm::HyperslabDataSelection( fileSelectionSlab ) ) );
    dataset->serialize( mStructuredArray.get(), selectionMap );
  }
}

void FunctionData::readImplementation( xdm::Dataset* dataset ) {
  XDM_THROW( std::runtime_error( "Not implemented" ) );
}

//-----------------------------------------------------------------------------
double TestCaseFunction::operator()( double x, double y, double z ) {
  using std::sin;
  return sin( x + 2*y + 4*z );
}

