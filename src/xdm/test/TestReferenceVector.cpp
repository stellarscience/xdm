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
#define BOOST_TEST_MODULE ReferenceVector
#include <boost/test/unit_test.hpp>

#include <xdm/ReferenceVector.hpp>

#include <cmath>
#include <vector>

namespace {

BOOST_AUTO_TEST_CASE( singleArray )
{
  double data[20]; // to be indexed as [10][2], i.e. 10 2D vectors.
  data[10] = 0.25;
  data[11] = 0.75;

  xdm::RefPtr< xdm::SingleArrayOfVectorsImpl< double > > imp(
    new xdm::SingleArrayOfVectorsImpl< double >( data, 2 ) );

  {
    xdm::ReferenceVector< double > thinVector( imp, 5 );

    BOOST_CHECK_EQUAL( 2, thinVector.size() );
    BOOST_CHECK_EQUAL( 0.25, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.75, thinVector[1] );

    // Change some values before thinVector goes out of scope.
    thinVector[0] = 1.0;
    thinVector[1] = 0.5;
  }

  // thinVector is gone, but data remains. Make up a new vector and pass it the
  // same shared imp.

  {
    xdm::ReferenceVector< double > thinVector( imp, 5 );

    BOOST_CHECK_EQUAL( 1.0, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.5, thinVector[1] );
  }
}

BOOST_AUTO_TEST_CASE( multiArray )
{
  double x[10];
  double y[10];
  x[5] = 0.25;
  y[5] = 0.75;

  std::vector< double* > arrays;
  arrays.push_back( x );
  arrays.push_back( y );
  xdm::RefPtr< xdm::MultipleArraysOfVectorElementsImpl< double > > imp(
    new xdm::MultipleArraysOfVectorElementsImpl< double >( arrays ) );

  {
    xdm::ReferenceVector< double > thinVector( imp, 5 );

    BOOST_CHECK_EQUAL( 2, thinVector.size() );
    BOOST_CHECK_EQUAL( 0.25, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.75, thinVector[1] );

    // Change some values before thinVector goes out of scope.
    thinVector[0] = 1.0;
    thinVector[1] = 0.5;
  }

  // thinVector is gone, but data remains. Make up a new vector and pass it the
  // same shared imp.

  {
    xdm::ReferenceVector< double > thinVector( imp, 5 );

    BOOST_CHECK_EQUAL( 1.0, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.5, thinVector[1] );
  }
}

BOOST_AUTO_TEST_CASE( tensorProduct )
{
  double x[10];
  double y[7];
  x[5] = 0.25;
  y[3] = 0.75;

  std::vector< double* > coordinateAxisValues;
  coordinateAxisValues.push_back( x );
  coordinateAxisValues.push_back( y );
  std::vector< std::size_t > axisSizes;
  axisSizes.push_back( 10 );
  axisSizes.push_back( 7 );
  xdm::RefPtr< xdm::TensorProductArraysImpl< double > > imp(
    new xdm::TensorProductArraysImpl< double >( coordinateAxisValues, axisSizes ) );

  // If the tensor product values look as if they are contiguous, then the value
  // of interest lies at i = 5 and j = 3. Thus, base of this vector should look
  // like it is located at 10 * 3 + 5.

  {
    xdm::ReferenceVector< double > thinVector( imp, 10 * 3 + 5  );

    BOOST_CHECK_EQUAL( 2, thinVector.size() );
    BOOST_CHECK_EQUAL( 0.25, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.75, thinVector[1] );

    // Change some values before thinVector goes out of scope.
    thinVector[0] = 1.0;
    thinVector[1] = 0.5;
  }

  // thinVector is gone, but data remains. Make up a new vector and pass it the
  // same shared imp.

  {
    xdm::ReferenceVector< double > thinVector( imp, 10 * 3 + 5 );

    BOOST_CHECK_EQUAL( 1.0, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.5, thinVector[1] );
  }
}

BOOST_AUTO_TEST_CASE( copyConstruct )
{
  double data[20]; // to be indexed as [10][2], i.e. 10 2D vectors.
  xdm::RefPtr< xdm::SingleArrayOfVectorsImpl< double > > imp(
    new xdm::SingleArrayOfVectorsImpl< double >( data, 2 ) );

  {
    xdm::ReferenceVector< double > thinVector( imp, 5 );
    thinVector[0] = 0.25;
    thinVector[1] = 0.75;

    // Now copy thinVector. The reference should point to the original data set.
    xdm::ReferenceVector< double > copiedVector( thinVector );
    BOOST_CHECK_EQUAL( 2, copiedVector.size() );
    BOOST_CHECK_EQUAL( 0.25, copiedVector[0] );
    BOOST_CHECK_EQUAL( 0.75, copiedVector[1] );

    // Change some values before copiedVector goes out of scope.
    copiedVector[0] = 1.0;
    copiedVector[1] = 0.5;
  }

  {
  // copiedVector is gone, but data remains. Make up a new vector and pass it the
  // same shared imp.
  xdm::ReferenceVector< double > thinVector( imp, 5 );

    BOOST_CHECK_EQUAL( 1.0, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.5, thinVector[1] );
  }
}

BOOST_AUTO_TEST_CASE( assignment )
{
  double data[20]; // to be indexed as [10][2], i.e. 10 2D vectors.
  double garbage[20];
  xdm::RefPtr< xdm::SingleArrayOfVectorsImpl< double > > imp(
    new xdm::SingleArrayOfVectorsImpl< double >( data, 2 ) );
  xdm::RefPtr< xdm::SingleArrayOfVectorsImpl< double > > dummyImp(
    new xdm::SingleArrayOfVectorsImpl< double >( garbage, 2 ) );

  {
    xdm::ReferenceVector< double > thinVector( imp, 5 );
    thinVector[0] = 0.25;
    thinVector[1] = 0.75;

    // This vector points to an array that hasn't been initialized. It should contain
    // garbage.
    xdm::ReferenceVector< double > assignedVector( dummyImp, 7 );
    BOOST_CHECK_NE( 0.25, assignedVector[0] );
    BOOST_CHECK_NE( 0.75, assignedVector[1] );

    // Now assign thinVector. The reference should point to the original data set.
    assignedVector = thinVector;
    BOOST_CHECK_EQUAL( 2, assignedVector.size() );
    BOOST_CHECK_EQUAL( 0.25, assignedVector[0] );
    BOOST_CHECK_EQUAL( 0.75, assignedVector[1] );

    // Change some values before assignedVector goes out of scope.
    assignedVector[0] = 1.0;
    assignedVector[1] = 0.5;
  }

  {
  // assignedVector is gone, but data remains. Make up a new vector and pass it the
  // same shared imp.
  xdm::ReferenceVector< double > thinVector( imp, 5 );

    BOOST_CHECK_EQUAL( 1.0, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.5, thinVector[1] );
  }
}

BOOST_AUTO_TEST_CASE( dotProduct )
{
  // First, a vector from a single array.
  double data[30];
  data[9] =  0.1;
  data[10] = 0.2;
  data[11] = 0.3;
  xdm::RefPtr< xdm::SingleArrayOfVectorsImpl< double > > imp1(
    new xdm::SingleArrayOfVectorsImpl< double >( data, 3 ) );
  xdm::ReferenceVector< double > v1( imp1, 3 );

  // Now, a vector from a set of x/y arrays.
  double x[10];
  double y[10];
  double z[10];
  x[7] = 0.4;
  y[7] = 0.5;
  z[7] = 0.6;
  std::vector< double* > arrays;
  arrays.push_back( x );
  arrays.push_back( y );
  arrays.push_back( z );
  xdm::RefPtr< xdm::MultipleArraysOfVectorElementsImpl< double > > imp2(
    new xdm::MultipleArraysOfVectorElementsImpl< double >( arrays ) );
  xdm::ReferenceVector< double > v2( imp2, 7 );

  // Compute a dot product.
  double testDot = std::sqrt( v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] );
  double trueDot = std::sqrt( 0.1 * 0.4 + 0.2 * 0.5 + 0.3 * 0.6 );
  BOOST_CHECK_CLOSE( testDot, trueDot, 1.e-10 );
}

} // namespace

