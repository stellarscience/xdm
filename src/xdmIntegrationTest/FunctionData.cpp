#include <xdmIntegrationTest/FunctionData.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/TemplateStructuredArray.hpp>
#include <xdm/VectorStructuredArray.hpp>
#include <xdm/WritableArray.hpp>

#include <xdmGrid/UniformGrid.hpp>
#include <xdmGrid/RectilinearMesh.hpp>
#include <xdmGrid/TensorProductGeometry.hpp>
#include <xdmGrid/Time.hpp>

#include <xdmHdf/HdfDataset.hpp>

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
    bounds.mSize[2] + 1, 
    bounds.mSize[1] + 1,
    bounds.mSize[0] + 1 ) );
  grid->setTopology( topology.get() );

  // Geometry
  xdm::RefPtr< xdmGrid::TensorProductGeometry > geometry( 
    new xdmGrid::TensorProductGeometry( 3 ) );
  grid->setGeometry( geometry.get() );

  // assign geometry values
  for ( int i = 0; i < 3 ; i++ ) {
    xdm::RefPtr< xdm::UniformDataItem > dataItem( new xdm::UniformDataItem( 
      xdm::primitiveType::kDouble, xdm::makeShape( bounds.mSize[i] + 1 ) ) );
    // allocate enough space for bounds + 1 vertices to encompass the whole
    // range.
    xdm::RefPtr< xdm::VectorStructuredArray< double > > values( 
      new xdm::VectorStructuredArray< double >( bounds.mSize[i] + 1 ) );
    for ( int j = 0; j < bounds.mSize[i]; j++ ) {
      (*values)[j] = 
        bounds.mBounds[i].first + 
        j * ( bounds.mBounds[i].second - bounds.mBounds[i].first ) 
        / bounds.mSize[i];
    }
    // fill the final point
    (*values)[ bounds.mSize[i] ] = bounds.mBounds[i].second;
    dataItem->appendData( new xdm::WritableArray( values.get() ) );
    geometry->setCoordinateValues( i, dataItem.get() );
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
  attribute->dataItem()->setDataset( attributeDataset );
  
  return std::make_pair( grid.get(), attribute.get() );
}

//-----------------------------------------------------------------------------
FunctionData::FunctionData( 
  const GridBounds& grid, 
  const xdm::HyperSlab<>& region ) :
  mGrid( grid ),
  mRegionOfInterest( region ),
  mStorage(),
  mStructuredArray() {

  // reverse the order of the dimension specification in the grid and region of
  // interest since the data will be written in KJI order.
  reverseDimensionOrder( mGrid.mSize );
  reverseDimensionOrder( mRegionOfInterest );

  mStorage.resize( 
    mRegionOfInterest.count( 0 ) *
    mRegionOfInterest.count( 1 ) *
    mRegionOfInterest.count( 2 ) );
  mStructuredArray = xdm::createStructuredArray( 
    &mStorage[0], xdm::makeShape( mStorage.size() ) );
}

FunctionData::~FunctionData() {
}

void FunctionData::writeImplementation( xdm::Dataset* dataset ) {
  int sizeI = mRegionOfInterest.count(0);
  int sizeJ = mRegionOfInterest.count(1);
  int sizeK = mRegionOfInterest.count(2);
  for ( int k = 0; k < sizeK; k++ ) {
    for ( int j = 0; j < sizeJ; j++ ) {
      for ( int i = 0; i < sizeI; i++ ) {
        double result = evaluate( i, j, k );
        int arrayLocation = i + j*sizeI + k*sizeI*sizeJ;
        mStorage[arrayLocation] = result;
      }
    }
  }

  xdm::DataSelectionMap selectionMap(
    new xdm::AllDataSelection,
    new xdm::HyperslabDataSelection( mRegionOfInterest ) );
  dataset->serialize( mStructuredArray, selectionMap );
}

