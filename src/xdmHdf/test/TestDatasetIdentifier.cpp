//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE TestDatasetIdentifier
#include <boost/test/unit_test.hpp>

#include <xdmHdf/DatasetIdentifier.hpp>

#include <xdm/DatasetExcept.hpp>

#include <hdf5.h>

#include <stdexcept>

char const * const kDatasetName = "TestDataset";

class Fixture {
public:
  hid_t fileIdentifier;
  hid_t spaceIdentifier;
  xdm::RefPtr< xdmHdf::DatasetIdentifier > datasetIdentifier;

  Fixture() {
    fileIdentifier = H5Fcreate(
      "TestDatasetIdentifier.h5",
      H5F_ACC_TRUNC,
      H5P_DEFAULT,
      H5P_DEFAULT );
    hsize_t dims[] = {2, 2};
    spaceIdentifier = H5Screate_simple( 2, dims, NULL );
    datasetIdentifier = xdmHdf::createDatasetIdentifier(
      fileIdentifier,
      kDatasetName,
      H5T_NATIVE_FLOAT,
      spaceIdentifier,
      xdm::Dataset::kCreate );
  }

  ~Fixture() {
    datasetIdentifier.reset();
    H5Sclose( spaceIdentifier );
    H5Fclose( fileIdentifier );
  }
};

BOOST_AUTO_TEST_CASE( create ) {
  Fixture test;
  BOOST_REQUIRE_GE( test.datasetIdentifier->get(), 0 );
}

BOOST_AUTO_TEST_CASE( spaceMismatchThrows ) {
  // check for exception when space doesn't match
  Fixture test;

  hsize_t badDims[] = {3, 3};
  hid_t badSpace = H5Screate_simple( 2, badDims, NULL );
  BOOST_CHECK_THROW(
    xdmHdf::createDatasetIdentifier(
      test.fileIdentifier,
      kDatasetName,
      H5T_NATIVE_FLOAT,
      badSpace,
      xdm::Dataset::kRead ),
    xdm::DataspaceMismatch );
  H5Sclose( badSpace );
}

BOOST_AUTO_TEST_CASE( typeMismatchThrows ) {
  // check for exception when type doesn't match
  Fixture test;

  BOOST_CHECK_THROW(
    xdmHdf::createDatasetIdentifier(
      test.fileIdentifier,
      kDatasetName,
      H5T_NATIVE_INT,
      test.spaceIdentifier,
      xdm::Dataset::kRead ),
    xdm::DatatypeMismatch );
}
