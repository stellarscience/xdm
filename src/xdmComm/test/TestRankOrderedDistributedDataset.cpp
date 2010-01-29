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
#define BOOST_TEST_MODULE RankOrderedDistributedDataset
#include <boost/test/unit_test.hpp>

#include <xdmComm/RankOrderedDistributedDataset.hpp>

#include <xdmComm/test/MpiTestFixture.hpp>

#include <xdm/HyperslabDataSelection.hpp>

#include <mpi.h>

#include <string>

namespace {

xdmComm::test::MpiTestFixture globalFixture;

class TestDataset : public xdm::Dataset {
public:
  std::string data;

  TestDataset() : data() {}
  virtual ~TestDataset() {}

  virtual const char* format() { return "Test"; }
  virtual void writeTextContent( xdm::XmlTextContent& ) {}

protected:
  virtual void initializeImplementation(
    xdm::primitiveType::Value type,
    const xdm::DataShape<> &shape,
    const xdm::Dataset::InitializeMode &mode ) {
    data.resize( shape[0] );
    std::fill( data.begin(), data.end(), 'x' );
  }

  virtual void serializeImplementation(
    const xdm::StructuredArray *,
    const xdm::DataSelectionMap & selectionMap ) {

    xdm::RefPtr< const xdm::DataSelection > disk( selectionMap.range() );

    xdm::RefPtr< const xdm::HyperslabDataSelection > diskSlab
      = xdm::dynamic_pointer_cast< const xdm::HyperslabDataSelection >( disk );
    BOOST_REQUIRE( diskSlab );

    std::size_t startIndex = diskSlab->hyperslab().start( 0 );
    std::size_t stride = diskSlab->hyperslab().stride( 0 );
    std::size_t count = diskSlab->hyperslab().count( 0 );

    for ( size_t i = 0; i < count; i++ ) {
      data[startIndex + i * stride] = 'a';
    }
  }

  virtual void deserializeImplementation(
    xdm::StructuredArray *,
    const xdm::DataSelectionMap & ) {
  }

  virtual void finalizeImplementation() {
  }
};

BOOST_AUTO_TEST_CASE( selectAllShift ) {
  xdm::RefPtr< TestDataset > result( new TestDataset );
  xdm::RefPtr< xdmComm::RankOrderedDistributedDataset > test(
    new xdmComm::RankOrderedDistributedDataset( result, MPI_COMM_WORLD ) );

  test->initialize(
    xdm::primitiveType::kChar,
    xdm::makeShape( 4 ),
    xdm::Dataset::kCreate );
  // even though only 4 elements were requested, the proxy should have initialized
  // with 4 elements per process for a larger dataset.
  BOOST_CHECK_EQUAL( result->data.size(), globalFixture.processes() * 4 );

  xdm::DataSelectionMap selectionMap; // default all to all selection
  test->serialize( 0, selectionMap );

  // the answer should have x's in everywhere except the 4 spots given to this
  // process.
  std::string answer;
  answer.resize( globalFixture.processes() * 4 );
  std::fill( answer.begin(), answer.end(), 'x' );
  std::size_t start = globalFixture.localRank() * 4;
  std::fill( &answer[start], &answer[start] + 4, 'a' );

  BOOST_CHECK_EQUAL( answer, result->data );
}

BOOST_AUTO_TEST_CASE( selectHyperslabShift ) {
  xdm::RefPtr< TestDataset > result( new TestDataset );
  xdm::RefPtr< xdmComm::RankOrderedDistributedDataset > test(
    new xdmComm::RankOrderedDistributedDataset( result, MPI_COMM_WORLD ) );

  test->initialize(
    xdm::primitiveType::kChar,
    xdm::makeShape( 4 ),
    xdm::Dataset::kCreate );
  // the proxy should have initialized with 8 elements per process for the
  // gathered dataset.
  BOOST_CHECK_EQUAL( result->data.size(), globalFixture.processes() * 4 );

  xdm::HyperSlab<> selectionSlab( xdm::makeShape( 4 ) );
  selectionSlab.setStart( 0, 0 );
  selectionSlab.setStride( 0, 2 );
  selectionSlab.setCount( 0, 2 );
  xdm::DataSelectionMap selectionMap;
  selectionMap.setRange( xdm::makeRefPtr(
    new xdm::HyperslabDataSelection( selectionSlab ) ) );

  test->serialize( 0, selectionMap );

  // the answer should have x's except over the 4 positions for this process,
  // having alternating a's and x's in those 4 places.
  std::string answer;
  answer.resize( globalFixture.processes() * 4 );
  std::fill( answer.begin(), answer.end(), 'x' );
  answer[globalFixture.localRank() * 4 + 0]  = 'a';
  answer[globalFixture.localRank() * 4 + 2]  = 'a';

  BOOST_CHECK_EQUAL( answer, result->data );
}

} // namespace
