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
#define BOOST_TEST_MODULE MemoryAdapter
#include <boost/test/unit_test.hpp>

#include <xdm/Dataset.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/MemoryAdapter.hpp>

namespace {

class DatasetTestImplementation : public xdm::Dataset {
public:
  bool dataWritten;
  DatasetTestImplementation() : xdm::Dataset(), dataWritten(false) {}

  xdm::DataShape<> initializeImplementation(
    xdm::primitiveType::Value,
    const xdm::DataShape<>&,
    const Dataset::InitializeMode& ) {
    return xdm::DataShape<>();
  }

  void serializeImplementation(
    const xdm::StructuredArray*, const xdm::DataSelectionMap& )
  {}

  void deserializeImplementation(
    xdm::StructuredArray*, const xdm::DataSelectionMap& )
  {}

  void finalizeImplementation() {}

  const char* format() { return "test"; }
  void writeTextContent(xdm::XmlTextContent&) {}
};

class MemoryAdapterTestImplementation : public xdm::MemoryAdapter {
public:
  virtual xdm::RefPtr< const xdm::StructuredArray > array() const {
    // not implemented, return invalid data.
    return xdm::RefPtr< const xdm::StructuredArray >();
  }

  virtual void writeImplementation( xdm::Dataset* dataset ) {
    DatasetTestImplementation* ds =
      dynamic_cast< DatasetTestImplementation* >( dataset );
    ds->dataWritten=true;
  }
};

class Fixture {
public:
  xdm::RefPtr< DatasetTestImplementation > testDataset;
  xdm::RefPtr< MemoryAdapterTestImplementation > testWritable;
  Fixture() :
    testDataset( new DatasetTestImplementation ),
    testWritable( new MemoryAdapterTestImplementation ) {}
  ~Fixture() {}
};

BOOST_AUTO_TEST_CASE( dynamicWrite ) {
  Fixture test;

  test.testWritable->setIsDynamic(true);

  BOOST_CHECK( !test.testDataset->dataWritten );

  test.testWritable->write( test.testDataset.get() );
  BOOST_CHECK( test.testDataset->dataWritten );

  test.testDataset->dataWritten = false;
  test.testWritable->write( test.testDataset.get() );
  BOOST_CHECK( test.testDataset->dataWritten );
}

BOOST_AUTO_TEST_CASE( staticWrite ) {
  Fixture test;

  test.testWritable->setIsDynamic( false );

  BOOST_CHECK( !test.testDataset->dataWritten );

  // should write the first time
  test.testWritable->write( test.testDataset.get() );
  BOOST_CHECK( test.testDataset->dataWritten );

  // should not write the second time
  test.testDataset->dataWritten = false;
  test.testWritable->write( test.testDataset.get() );
  BOOST_CHECK( !test.testDataset->dataWritten );

  // should write when needs update is set
  test.testWritable->setNeedsUpdate(true);
  test.testDataset->dataWritten = false;
  test.testWritable->write( test.testDataset.get() );
  BOOST_CHECK( test.testDataset->dataWritten );
}

} // namespace

