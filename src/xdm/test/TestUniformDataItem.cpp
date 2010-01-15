#define BOOST_TEST_MODULE UniformDataItem 
#include <boost/test/unit_test.hpp>

#include <xdm/TemplateStructuredArray.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/WritableArray.hpp>

#include <algorithm>

// dummy dataset implementation for testing.
class DummyDataset : public xdm::Dataset {
  const char* format() { return "Dummy"; }
  void writeTextContent( xdm::XmlTextContent& text ) {
    text.appendContentLine( "DummyDatasetInitializationContent" );
  }
  void initializeImplementation(
    xdm::primitiveType::Value,
    const xdm::DataShape<>& ) {}
  void serializeImplementation(
    const xdm::StructuredArray*,
    const xdm::DataSelectionMap& ) {}
  void finalizeImplementation() {}
};

// Fixture defines a common UniformDataItem for testing.
class Fixture {
public:
  float data[4];
  xdm::RefPtr< xdm::UniformDataItem > testItem;

  Fixture() :
    data(),
    testItem() {
    std::fill( data, data + 4, 0.0 );
    xdm::DataShape<> arrayShape(1);
    arrayShape[0] = 4;
    xdm::RefPtr< xdm::StructuredArray > array( 
      createStructuredArray( data, arrayShape ) );
    xdm::RefPtr< xdm::Dataset > dataset( new DummyDataset );

    xdm::DataShape<> datasetShape(2);
    datasetShape[0] = datasetShape[1] = 2;
    testItem = new xdm::UniformDataItem( 
      xdm::primitiveType::kFloat, 
      datasetShape );

    testItem->appendData( new xdm::WritableArray( array ) );
    testItem->setDataset( dataset );
  }
};


BOOST_AUTO_TEST_CASE( writeMetadata ) {
  Fixture test;
  
  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml( obj );
  test.testItem->writeMetadata( xml );

  BOOST_CHECK_EQUAL( xml.tag(), "DataItem" );
  BOOST_CHECK_EQUAL( xml.attribute( "ItemType" ), "Uniform" );
  BOOST_CHECK_EQUAL( xml.attribute( "Dimensions" ), "2 2" );
  BOOST_CHECK_EQUAL( xml.attribute( "NumberType" ), "Float" );
  BOOST_CHECK_EQUAL( xml.attribute( "Precision" ), "4" );
  BOOST_CHECK_EQUAL( xml.attribute( "Format" ), "Dummy" );
  BOOST_CHECK_EQUAL( "DummyDatasetInitializationContent", xml.contentLine( 0 ) );
}

