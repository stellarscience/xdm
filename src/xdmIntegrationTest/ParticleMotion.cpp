#define BOOST_TEST_MODULE ParticleMotion
#include <boost/test/unit_test.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/VectorStructuredArray.hpp>
#include <xdm/WritableArray.hpp>

#include <xdmComm/ParallelizeTreeVisitor.hpp>

#include <xdmComm/test/MpiTestFixture.hpp>

#include <xdmFormat/TemporalCollection.hpp>
#include <xdmFormat/VirtualDataset.hpp>

#include <xdmGrid/InterlacedGeometry.hpp>
#include <xdmGrid/Polyvertex.hpp>
#include <xdmGrid/Time.hpp>
#include <xdmGrid/UniformGrid.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <algorithm>
#include <iomanip>
#include <sstream>

#include <cmath>
#include <cstdlib>

namespace {

xdmComm::test::MpiTestFixture globalFixture;

const unsigned int kParticleCount = 1000;
const float kEndTime = 1.0f;
const unsigned int kSteps = 1000;

// Point structure, just for passing data
struct Point {
  float x;
  float y;
  float z;
};

Point evolve( const Point& current, float dt ) {
  Point result;
  result.x = current.x + dt * 10.0f * ( current.y - current.x );
  result.y = current.y + dt * ( current.x - current.x*current.z - current.y );
  result.z = current.z + dt * ( current.x*current.y - 2.6667*current.z );
  return result;
}

// Update callback to tell a HDF5 dataset to rename itself on every time step
class NameDataset : public xdm::DatasetUpdateCallback< xdmHdf::HdfDataset > {
private:
  unsigned int mCount;

public:
  NameDataset() : mCount( 0 ) {}

  virtual void update( xdmHdf::HdfDataset* dataset ) {
    std::stringstream name;
    name << std::setfill( '0' ) << std::setw( 7 ) << mCount++;
    dataset->setDataset( name.str() );
  }
};

BOOST_AUTO_TEST_CASE( writeResult ) {
  std::stringstream baseName;
  baseName << "ParticleMotion.parallel."<< globalFixture.processes();

  // split the particles roughly equally among the total number of processes
  int particlesPerProcess = kParticleCount / globalFixture.processes();
  int remainingParticles = kParticleCount % globalFixture.processes();

  // distribute the remainder among the first few processes
  int localParticles = particlesPerProcess;
  int localStartIndex = globalFixture.localRank() * particlesPerProcess;
  if ( globalFixture.localRank() < remainingParticles ) {
    localParticles += 1;
    localStartIndex += globalFixture.localRank();
  } else {
    localStartIndex += remainingParticles;
  }

  // allocate an array for the local process' particle data
  xdm::RefPtr< xdm::VectorStructuredArray< float > > mPositions(
    new xdm::VectorStructuredArray< float >( 3 * localParticles ) );
  // generate some initial values
  for ( int i = 0; i < localParticles; i++ ) {
    float theta = ( 6.28 / kParticleCount ) * ( localStartIndex + i );
    (*mPositions)[3*i + 0] = std::cos( theta );
    (*mPositions)[3*i + 1] = std::sin( theta );
    (*mPositions)[3*i + 2] = 0.0f;
  }

  // build the dynamic grid tree
  xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid() );
  grid->setTime( new xdmGrid::Time );

  // topology is polyvertex
  xdm::RefPtr< xdmGrid::Polyvertex > topology( new xdmGrid::Polyvertex() );
  topology->setNumberOfElements( 1 );
  topology->setNodesPerElement( kParticleCount );
  grid->setTopology( topology.get() );

  // geometry is interlaced with a dynamic array data item
  xdm::RefPtr< xdmGrid::InterlacedGeometry > geometry(
    new xdmGrid::InterlacedGeometry( 3 ) );
  grid->setGeometry( geometry.get() );

  // define the shape of the data as it will be output on disk, an array of
  // three values each.
  xdm::DataShape<> outputShape = xdm::makeShape( kParticleCount, 3 );

  // data item for the geometry coordinate values
  xdm::RefPtr< xdm::UniformDataItem > geometryData( new xdm::UniformDataItem(
    xdm::primitiveType::kFloat, outputShape ) );
  geometry->setCoordinateValues( geometryData );

  // WritableData for the geometry coordinate values
  xdm::RefPtr< xdm::WritableArray > geometryMemory( new xdm::WritableArray(
    mPositions.get() ) );
  geometryMemory->setIsDynamic( true ); // changes throughout the simulation
  geometryData->appendData( geometryMemory.get() );

  // Set up the memory's data selection to map from the entire array to the
  // subset of the output dataset beginning at my local start index.
  xdm::HyperSlab<> datasetSlab( outputShape );
  datasetSlab.setStart( 0, localStartIndex ); // where in the array of points to start
  datasetSlab.setStride( 0, 1 ); // select a contiguous set of points
  datasetSlab.setCount( 0, localParticles ); // select the right number of points
  // second dimension is point coordinates, select all of them.
  datasetSlab.setStart( 1, 0 );
  datasetSlab.setStride( 1, 1 );
  datasetSlab.setCount( 1, 3 );
  xdm::DataSelectionMap mapping(
    new xdm::AllDataSelection, // select the entire array
    new xdm::HyperslabDataSelection( datasetSlab ) ); // only the portion I own
  geometryMemory->setSelectionMap( mapping );

  // put the geometry memory in an HDF dataset
  xdm::RefPtr< xdmHdf::HdfDataset > geometryDataset( new xdmHdf::HdfDataset );
  xdmHdf::GroupPath datasetPath( 1, "points" );
  geometryDataset->setFile( baseName.str() + ".h5" );
  geometryDataset->setGroupPath( datasetPath );
  // install the callback to set a new dataset name every timestep
  geometryDataset->setUpdateCallback( new NameDataset );
  geometryData->setDataset( geometryDataset.get() );

  // parallelize the tree
  xdmComm::ParallelizeTreeVisitor parallelize(
    (3 * kParticleCount + 1) * sizeof( float ) );
  grid->accept( parallelize );

  // create the time series, opening the output stream
  xdm::RefPtr< xdmFormat::TimeSeries > series(
    new xdmFormat::TemporalCollection( baseName.str() + ".xmf" ) );

  // begin a time series
  series->open();

  // write the first timestep
  writeTimestepGrid( series, grid.get() );

  // evolve the particles
  float t = 0.0f;
  float dt = kEndTime / kSteps;
  float cosDt = kEndTime;
  for ( unsigned int i = 0; i < kSteps; i++ ) {
    t += dt;
    grid->time()->setValue( t );
    for ( unsigned int i = 0; i < localParticles; i++ ) {
      Point current;
      current.x = (*mPositions)[i * 3 + 0];
      current.y = (*mPositions)[i * 3 + 1];
      current.z = (*mPositions)[i * 3 + 2];

      Point update = evolve( current, dt );
      (*mPositions)[i * 3 + 0] = update.x;
      (*mPositions)[i * 3 + 1] = update.y;
      (*mPositions)[i * 3 + 2] = update.z;
    }

    // write the data for this timestep to disk.
    writeTimestepGrid( series, grid.get() );
  }

  series->close();

}

} // namespace
