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
#define BOOST_TEST_MODULE ParticleMotion
#include <boost/test/unit_test.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/FileSystem.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/VectorStructuredArray.hpp>
#include <xdm/ArrayAdapter.hpp>

#include <xdmComm/ParallelizeTreeVisitor.hpp>

#include <xdmComm/test/MpiTestFixture.hpp>

#include <xdmFormat/LinearTopologyData.hpp>
#include <xdmFormat/TemporalCollection.hpp>
#include <xdmFormat/VirtualDataset.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/InterlacedGeometry.hpp>
#include <xdmGrid/Polyvertex.hpp>
#include <xdmGrid/Time.hpp>
#include <xdmGrid/UniformGrid.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <algorithm>
#include <limits>
#include <iomanip>
#include <sstream>

#include <cmath>
#include <cstdlib>

namespace {

xdmComm::test::MpiTestFixture globalFixture;

const unsigned int kParticleCount = 1000;
const float kEndTime = 1.0f;
const unsigned int kSteps = 1000;

// get the square of the magnitude of a vector.
float magnitudeSquared( float* v ) {
  return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
}

// evolve according to an inverse square force law. Returns false if
// the object gets too close to the center and the object is removed.
bool evolve( float* position, float* velocity, float dt ) {
  float rSquared = magnitudeSquared( position );
  // threshold r^2 to avoid a division by 0.
  if ( rSquared < 0.001f ) {
    rSquared = 0.001f;
  }
  for ( int i = 0; i < 3; i++ ) {
    float rHat = position[i] / rSquared;
    velocity[i] = velocity[i] + dt * ( -1.0f / rSquared ) * rHat;
    position[i] = position[i] + dt * velocity[i];
  }
  return true;
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

  const std::string xmfFile = baseName.str() + ".xmf";
  const std::string hdfFile = baseName.str() + ".h5";

  if ( globalFixture.localRank() == 0 ) {
    xdm::remove( xdm::FileSystemPath( xmfFile ) );
    xdm::remove( xdm::FileSystemPath( hdfFile ) );
  }
  globalFixture.waitAll();

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

  // allocate an array for the local process' particle position data
  xdm::RefPtr< xdm::VectorStructuredArray< float > > mPositions(
    new xdm::VectorStructuredArray< float >( 3 * localParticles ) );
  // generate some initial values
  for ( int i = 0; i < localParticles; i++ ) {
    unsigned int globalIndex = localStartIndex + i;
    float theta = ( 6.28 / kParticleCount ) * ( globalIndex );
    (*mPositions)[3*i + 0] = std::cos( theta );
    (*mPositions)[3*i + 1] = 0.5f * std::sin( theta );
    (*mPositions)[3*i + 2] = 0.0f;
  }

  // allocate an array for the local process' particle velocity data and
  // initialize the values to 0.
  xdm::RefPtr< xdm::VectorStructuredArray< float > > mVelocities(
    new xdm::VectorStructuredArray< float >( 3 * localParticles ) );
  std::fill( mVelocities->begin(), mVelocities->end(), 0.0f );

  // build the dynamic grid tree
  xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid() );
  grid->setTime( xdm::makeRefPtr( new xdmGrid::Time ) );

  // topology is polyvertex
  xdm::RefPtr< xdmGrid::Polyvertex > topology( new xdmGrid::Polyvertex() );
  topology->setNumberOfPoints( kParticleCount );
  xdm::RefPtr< xdm::UniformDataItem > topologyConn =
    xdmFormat::createLinearTopologyUniformDataItem( topology );
  topology->setConnectivity( topologyConn );
  xdm::RefPtr< xdmHdf::HdfDataset > topologyDataset( new xdmHdf::HdfDataset );
  topologyDataset->setFile( baseName.str() + ".h5" );
  topologyDataset->setGroupPath( xdmHdf::GroupPath( 1, "connectivity" ) );
  topologyDataset->setUpdateCallback( xdm::makeRefPtr( new NameDataset ) );
  topologyConn->setDataset( topologyDataset );
  grid->setTopology( topology );

  // geometry is interlaced with a dynamic array data item
  xdm::RefPtr< xdmGrid::InterlacedGeometry > geometry(
    new xdmGrid::InterlacedGeometry( 3 ) );
  grid->setGeometry( geometry );

  // define the shape of the data as it will be output on disk, an array of
  // three values each.
  xdm::DataShape<> outputShape = xdm::makeShape( kParticleCount, 3 );

  // data item for the geometry coordinate values
  xdm::RefPtr< xdm::UniformDataItem > geometryData( new xdm::UniformDataItem(
    xdm::primitiveType::kFloat, outputShape ) );
  geometry->setCoordinateValues( geometryData );

  // MemoryAdapter for the geometry coordinate values
  xdm::RefPtr< xdm::ArrayAdapter > geometryMemory( new xdm::ArrayAdapter(
    mPositions ) );
  geometryMemory->setIsDynamic( true ); // changes throughout the simulation
  geometryData->setData( geometryMemory );

  // put the geometry memory in an HDF dataset
  xdm::RefPtr< xdmHdf::HdfDataset > geometryDataset( new xdmHdf::HdfDataset );
  xdmHdf::GroupPath datasetPath( 1, "points" );
  geometryDataset->setFile( hdfFile );
  geometryDataset->setGroupPath( datasetPath );
  // install the callback to set a new dataset name every timestep
  geometryDataset->setUpdateCallback( xdm::makeRefPtr( new NameDataset ) );
  geometryData->setDataset( geometryDataset );

  // create a vector attribute for the velocity.
  xdm::RefPtr< xdmGrid::Attribute > velocityAttribute( 
    new xdmGrid::Attribute( xdmGrid::Attribute::kVector ) );
  velocityAttribute->setName( "velocity" );
  grid->addAttribute( velocityAttribute );
  // create the data item for the velocity attribute.
  xdm::RefPtr< xdm::UniformDataItem > velocityData( new xdm::UniformDataItem(
    xdm::primitiveType::kFloat, outputShape ) );
  velocityAttribute->setDataItem( velocityData );
  // create dynamic data for the velocity values.
  xdm::RefPtr< xdm::ArrayAdapter > velocityMemory( new xdm::ArrayAdapter(
    mVelocities ) );
  velocityMemory->setIsDynamic( true );
  velocityData->setData( velocityMemory );
  // put the velocities in an HDF dataset.
  xdm::RefPtr< xdmHdf::HdfDataset > velocityDataset( new xdmHdf::HdfDataset );
  xdmHdf::GroupPath velGroup( 1, "velocities" );
  velocityDataset->setFile( hdfFile );
  velocityDataset->setGroupPath( velGroup );
  velocityDataset->setUpdateCallback( xdm::makeRefPtr( new NameDataset ) );
  velocityData->setDataset( velocityDataset );

  
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
    xdm::makeRefPtr( new xdm::AllDataSelection ), // select the entire array
    xdm::makeRefPtr( new xdm::HyperslabDataSelection( datasetSlab ) ) ); // only the portion I own
  // The selection map applies to both the positions and velocities of the
  // particles, so we can use it for both memory mappings.
  geometryMemory->setSelectionMap( mapping );
  velocityMemory->setSelectionMap( mapping );


  // parallelize the tree
  // Choose a small buffer size to force buffering of the data.
  xdmComm::ParallelizeTreeVisitor parallelize( sizeof( float ) );
  grid->accept( parallelize );

  // create the time series, opening the output stream
  xdm::RefPtr< xdmFormat::TimeSeries > series(		      
    new xdmFormat::VirtualDataset( xmfFile, xdm::Dataset::kCreate ) );

  // begin a time series
  series->open();

  // write the first timestep
  writeTimestepGrid( series, grid );

  // evolve the particles
  float t = 0.0f;
  float dt = kEndTime / kSteps;
  float cosDt = kEndTime;
  for ( unsigned int i = 0; i < kSteps; i++ ) {
    t += dt;
    grid->time()->setValue( t );
    
    // particle update loop
    for ( unsigned int i = 0; i < localParticles; i++ ) {
      evolve( &(*mPositions)[3*i], &(*mVelocities)[3*i], dt );
    }

    // write the data for this timestep to disk.
    writeTimestepGrid( series, grid );
  }

  series->close();

}

} // namespace
