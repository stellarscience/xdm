//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE ParticleMotion
#include <boost/test/unit_test.hpp>

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

#include <sstream>

#include <cstdlib>

namespace {

xdmComm::test::MpiTestFixture globalFixture;

const unsigned int kParticleCount = 100;

BOOST_AUTO_TEST_CASE( writeResult ) {
  std::stringstream baseName;
  baseName << "ParticleMotion.parallel."<< globalFixture.processes();

  // split the particles roughly equally among the total number of processes
  int particlesPerProcess = kParticleCount / globalFixture.processes();
  int remainingParticles = kParticleCount % globalFixture.processes();

  // distribute the remainder among the first few processes
  int localParticles = particlesPerProcess;
  if ( globalFixture.localRank() < remainingParticles ) {
    localParticles += 1;
  }

  // allocate an array for the local process' particle data
  xdm::RefPtr< xdm::VectorStructuredArray< float > > mPositions(
    new xdm::VectorStructuredArray< float >( 3 * localParticles ) );

  // generate random starting locations for the particles
  srand( 42 );
  for ( int i = 0; i < localParticles; i++ ) {
    float x = ( (float)rand() / (float)RAND_MAX ) - 0.5f;
    float y = ( (float)rand() / (float)RAND_MAX ) - 0.5f;
    (*mPositions)[i * 3 + 0] = x;
    (*mPositions)[i * 3 + 1] = y;
    (*mPositions)[i * 3 + 2] = 0.0f;
  }

  // build the dynamic grid tree
  xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid() );
  grid->setTime( new xdmGrid::Time( 0.0 ) );

  // topology is polyvertex
  xdm::RefPtr< xdmGrid::Polyvertex > topology( new xdmGrid::Polyvertex() );
  topology->setNumberOfElements( 1 );
  topology->setNodesPerElement( kParticleCount );
  grid->setTopology( topology.get() );

  // geometry is interlaced with a dynamic array data item
  xdm::RefPtr< xdmGrid::InterlacedGeometry > geometry(
    new xdmGrid::InterlacedGeometry( 3 ) );
  grid->setGeometry( geometry.get() );

  // data item for the geometry coordinate values
  xdm::RefPtr< xdm::UniformDataItem > geometryData( new xdm::UniformDataItem(
    xdm::primitiveType::kFloat, xdm::makeShape( kParticleCount, 3 ) ) );
  geometry->setCoordinateValues( geometryData );

  // WritableData for the geometry coordinate values
  xdm::RefPtr< xdm::WritableData > geometryMemory( new xdm::WritableArray(
    mPositions.get() ) );
  geometryMemory->setIsDynamic( true );
  geometryData->appendData( geometryMemory );

  // put the geometry memory in an HDF dataset
  xdm::RefPtr< xdmHdf::HdfDataset > geometryDataset( new xdmHdf::HdfDataset );
  xdmHdf::GroupPath datasetPath( 1, "points" );
  geometryDataset->setFile( baseName.str() + ".h5" );
  geometryDataset->setGroupPath( datasetPath );
  geometryDataset->setDataset( "values" );
  geometryData->setDataset( geometryDataset );

  // parallelize the tree
  /*xdmComm::ParallelizeTreeVisitor parallelize(
    (3 * kParticleCount + 1) * sizeof( float ) );
  grid->accept( parallelize );*/

  xdm::RefPtr< xdmFormat::TimeSeries > series(
    new xdmFormat::TemporalCollection( baseName.str() + ".xmf" ) );

  series->open();
  writeTimestepGrid( series, grid.get() );
  //writeTimestepGrid( series, grid.get() );
  series->close();

}

} // namespace
