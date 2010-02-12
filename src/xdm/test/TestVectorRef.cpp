//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
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
#define BOOST_TEST_MODULE VectorRef
#include <boost/test/unit_test.hpp>

#include <xdm/VectorRef.hpp>

#include <cmath>
#include <vector>

namespace {

BOOST_AUTO_TEST_CASE( singleArray )
{
  double data[20]; // to be indexed as [10][2], i.e. 10 2D vectors.
  data[10] = 0.25;
  data[11] = 0.75;

  xdm::RefPtr< xdm::SingleArrayOfVectorsImp< double > > imp(
    new xdm::SingleArrayOfVectorsImp< double >( data, 2 ) );

  {
    // Check indexing with non-const.
    xdm::VectorRef< double > thinVector( imp, 5 );

    BOOST_CHECK_EQUAL( 2, thinVector.size() );
    BOOST_CHECK_EQUAL( 0.25, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.75, thinVector[1] );

    // Check indexing with const.
    xdm::ConstVectorRef< double > constThinVector( imp, 5 );

    BOOST_CHECK_EQUAL( 2, constThinVector.size() );
    BOOST_CHECK_EQUAL( 0.25, constThinVector[0] );
    BOOST_CHECK_EQUAL( 0.75, constThinVector[1] );

    // Change some values before thinVector goes out of scope.
    thinVector[0] = 1.0;
    thinVector[1] = 0.5;
  }

  // thinVector is gone, but data remains. Make up a new vector and pass it the
  // same shared imp.

  {
    xdm::ConstVectorRef< double > thinVector( imp, 5 );

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
  xdm::RefPtr< xdm::MultipleArraysOfVectorElementsImp< double > > imp(
    new xdm::MultipleArraysOfVectorElementsImp< double >( arrays ) );

  {
    xdm::VectorRef< double > thinVector( imp, 5 );

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
    xdm::ConstVectorRef< double > thinVector( imp, 5 );

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
  xdm::RefPtr< xdm::TensorProductArraysImp< double > > imp(
    new xdm::TensorProductArraysImp< double >( coordinateAxisValues, axisSizes ) );

  // If the tensor product values look as if they are contiguous, then the value
  // of interest lies at i = 5 and j = 3. Thus, base of this vector should look
  // like it is located at 10 * 3 + 5.

  {
    xdm::VectorRef< double > thinVector( imp, 10 * 3 + 5  );

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
    xdm::ConstVectorRef< double > thinVector( imp, 10 * 3 + 5 );

    BOOST_CHECK_EQUAL( 1.0, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.5, thinVector[1] );
  }
}

BOOST_AUTO_TEST_CASE( copyConstruct )
{
  double data[20]; // to be indexed as [10][2], i.e. 10 2D vectors.
  xdm::RefPtr< xdm::SingleArrayOfVectorsImp< double > > imp(
    new xdm::SingleArrayOfVectorsImp< double >( data, 2 ) );

  {
    xdm::VectorRef< double > thinVector( imp, 5 );
    thinVector[0] = 0.25;
    thinVector[1] = 0.75;

    // Now copy thinVector. The reference should point to the original data set.
    xdm::ConstVectorRef< double > constCopiedVector( thinVector );
    BOOST_CHECK_EQUAL( 2, constCopiedVector.size() );
    BOOST_CHECK_EQUAL( 0.25, constCopiedVector[0] );
    BOOST_CHECK_EQUAL( 0.75, constCopiedVector[1] );

    // Copy again, this time with non-const and change some values before
    // copiedVector goes out of scope.
    xdm::VectorRef< double > copiedVector( thinVector );
    copiedVector[0] = 1.0;
    copiedVector[1] = 0.5;
    BOOST_CHECK_EQUAL( 2, constCopiedVector.size() );
    BOOST_CHECK_EQUAL( 1.0, copiedVector[0] );
    BOOST_CHECK_EQUAL( 0.5, copiedVector[1] );
  }

  {
  // copiedVector is gone, but data remains. Make up a new vector and pass it the
  // same shared imp.
  xdm::VectorRef< double > thinVector( imp, 5 );

    BOOST_CHECK_EQUAL( 1.0, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.5, thinVector[1] );
  }
}

BOOST_AUTO_TEST_CASE( assignment )
{
  double data[20]; // to be indexed as [10][2], i.e. 10 2D vectors.
  double garbage[20];
  xdm::RefPtr< xdm::SingleArrayOfVectorsImp< double > > imp(
    new xdm::SingleArrayOfVectorsImp< double >( data, 2 ) );
  xdm::RefPtr< xdm::SingleArrayOfVectorsImp< double > > dummyImp(
    new xdm::SingleArrayOfVectorsImp< double >( garbage, 2 ) );

  {
    xdm::VectorRef< double > thinVector( imp, 5 );
    thinVector[0] = 0.25;
    thinVector[1] = 0.75;

    // This vector points to an array that hasn't been initialized. It should contain
    // garbage.
    xdm::VectorRef< double > assignedVector( dummyImp, 7 );
    xdm::ConstVectorRef< double > constAssignedVector( dummyImp, 7 );

    BOOST_CHECK_NE( 0.25, assignedVector[0] );
    BOOST_CHECK_NE( 0.75, assignedVector[1] );

    // Now assign thinVector. The reference points to dummyImp, but the values are
    // copied over.
    // constAssignedVector = assignedVector; // doesn't compile. Good.
    xdm::ConstVectorRef< double > constDummy2( dummyImp, 7 );
    // constAssignedVector = constDummy2; // Doesn't compile. Good.
    xdm::ConstVectorRef< double > constCopy( assignedVector ); // compiles...
    assignedVector = constAssignedVector;
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
    xdm::VectorRef< double > thinVector( dummyImp, 7 );

    BOOST_CHECK_EQUAL( 1.0, thinVector[0] );
    BOOST_CHECK_EQUAL( 0.5, thinVector[1] );
  }
}

BOOST_AUTO_TEST_CASE( removeConst )
{
  double data[20]; // to be indexed as [10][2], i.e. 10 2D vectors.
  xdm::RefPtr< xdm::SingleArrayOfVectorsImp< double > > imp(
    new xdm::SingleArrayOfVectorsImp< double >( data, 2 ) );

  xdm::VectorRef< double > origVec( imp, 5 );
  origVec[0] = 0.25;
  origVec[1] = 0.75;

  xdm::ConstVectorRef< double > constVec( origVec );
  BOOST_CHECK_EQUAL( 0.25, constVec[0] );
  BOOST_CHECK_EQUAL( 0.75, constVec[1] );

  // Now make a third vector that casts away constness of constVec but retains
  // the reference to the same array.
  xdm::VectorRef< double > mutableVec( constVec.removeConstness() );
  mutableVec[0] = 3.0;
  mutableVec[1] = 7.0;
  BOOST_CHECK_EQUAL( 3.0, constVec[0] );
  BOOST_CHECK_EQUAL( 7.0, constVec[1] );
}

BOOST_AUTO_TEST_CASE( dotProduct )
{
  // First, a vector from a single array.
  double data[30];
  data[9] =  0.1;
  data[10] = 0.2;
  data[11] = 0.3;
  xdm::RefPtr< xdm::SingleArrayOfVectorsImp< double > > imp1(
    new xdm::SingleArrayOfVectorsImp< double >( data, 3 ) );
  xdm::VectorRef< double > v1( imp1, 3 );

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
  xdm::RefPtr< xdm::MultipleArraysOfVectorElementsImp< double > > imp2(
    new xdm::MultipleArraysOfVectorElementsImp< double >( arrays ) );
  xdm::VectorRef< double > v2( imp2, 7 );

  // Compute a dot product.
  double testDot = std::sqrt( v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] );
  double trueDot = std::sqrt( 0.1 * 0.4 + 0.2 * 0.5 + 0.3 * 0.6 );
  BOOST_CHECK_CLOSE( testDot, trueDot, 1.e-10 );
}

} // anon namespace
