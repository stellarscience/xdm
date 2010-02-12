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
#include <xdm/TypedStructuredArray.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/ArrayAdapter.hpp>

#include <algorithm>

namespace test {

// dummy dataset implementation for testing.
class DummyDataset : public xdm::Dataset {
  const char* format() { return "Dummy"; }
  void writeTextContent( xdm::XmlTextContent& text ) {
    text.appendContentLine( "DummyDatasetInitializationContent" );
  }
  void initializeImplementation(
    xdm::primitiveType::Value,
    const xdm::DataShape<>&,
    const Dataset::InitializeMode& ) {}
  void serializeImplementation(
    const xdm::StructuredArray*,
    const xdm::DataSelectionMap& ) {}
  void deserializeImplementation(
    xdm::StructuredArray*,
    const xdm::DataSelectionMap& ) {}
  void finalizeImplementation() {}
};

// Function that builds a UniformDataItem from a pre-existing 1D array.
template< typename T >
xdm::RefPtr< xdm::UniformDataItem > createUniformDataItem(
  T* existingArray,
  std::size_t size,
  xdm::primitiveType::Value primType ) {

  xdm::RefPtr< xdm::StructuredArray > array(
    xdm::createStructuredArray( existingArray, size ) );
  xdm::RefPtr< xdm::Dataset > dataset( new DummyDataset );
  xdm::DataShape<> datasetShape(1);
  datasetShape[0] = size;
  xdm::RefPtr< xdm::UniformDataItem > returnItem(
    xdm::makeRefPtr( new xdm::UniformDataItem( primType, datasetShape ) ) );
  returnItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( array ) ) );
  returnItem->setDataset( dataset );
  return returnItem;
}

// Fixture defines a common UniformDataItem for testing.
class Fixture {
public:
  float data[4];
  xdm::RefPtr< xdm::UniformDataItem > testItem;

  Fixture() :
    data(),
    testItem() {
    std::fill( data, data + 4, 0.0 );
    testItem = createUniformDataItem( data, 4, xdm::primitiveType::kFloat );
    xdm::DataShape<> datasetShape(2);
    datasetShape[0] = datasetShape[1] = 2;
    testItem->setDataspace( datasetShape );
  }
};

} // namespace test
