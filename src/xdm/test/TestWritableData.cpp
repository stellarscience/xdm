#define BOOST_TEST_MODULE WritableData 
#include <boost/test/unit_test.hpp>

#include <xdm/Dataset.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/WritableData.hpp>

namespace {

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

class Fixture {
public:
  xdm::RefPtr< DatasetTestImplementation > testDataset;
  xdm::RefPtr< WritableDataTestImplementation > testWritable;
  Fixture() :
    testDataset( new DatasetTestImplementation ),
    testWritable( new WritableDataTestImplementation ) {}
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

