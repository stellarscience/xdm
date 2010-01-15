#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdm/Dataset.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/WritableData.hpp>

class DatasetTestImplementation : public xdm::Dataset {
public:
  bool dataWritten;
  DatasetTestImplementation() : xdm::Dataset(), dataWritten(false) {}
  
  void initializeImplementation( 
    xdm::primitiveType::Value, const xdm::DataShape<>& )
  {}

  void serializeImplementation( 
    const xdm::StructuredArray*, const xdm::DataSelectionMap& )
  {}

  void finalizeImplementation() {}

  const char* format() { return "test"; }
  void writeTextContent(xdm::XmlTextContent&) {}
};

class WritableDataTestImplementation : public xdm::WritableData {
public:
  virtual void writeImplementation( xdm::Dataset* dataset ) {
    DatasetTestImplementation* ds = 
      dynamic_cast< DatasetTestImplementation* >( dataset );
    ds->dataWritten=true;
  }
};

class WritableData : public ::testing::Test {
public:
  xdm::RefPtr< DatasetTestImplementation > testDataset;
  xdm::RefPtr< WritableDataTestImplementation > testWritable;
  WritableData() :
    testDataset( new DatasetTestImplementation ),
    testWritable( new WritableDataTestImplementation ) {}
  ~WritableData() {}
};

BOOST_AUTO_TEST_CASE( dynamicWrite ) {
  testWritable->setIsDynamic(true);
  
  ASSERT_FALSE( testDataset->dataWritten );

  testWritable->write( testDataset.get() );
  ASSERT_TRUE( testDataset->dataWritten );

  testDataset->dataWritten = false;
  testWritable->write( testDataset.get() );
  ASSERT_TRUE( testDataset->dataWritten );
}

BOOST_AUTO_TEST_CASE( staticWrite ) {
  testWritable->setIsDynamic( false );

  ASSERT_FALSE( testDataset->dataWritten );

  // should write the first time
  testWritable->write( testDataset.get() );
  ASSERT_TRUE( testDataset->dataWritten );

  // should not write the second time
  testDataset->dataWritten = false;
  testWritable->write( testDataset.get() );
  ASSERT_FALSE( testDataset->dataWritten );

  // should write when needs update is set
  testWritable->setNeedsUpdate(true);
  testDataset->dataWritten = false;
  testWritable->write( testDataset.get() );
  ASSERT_TRUE( testDataset->dataWritten );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

