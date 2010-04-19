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
#ifndef xdmIntegrationTest_FunctionData_hpp
#define xdmIntegrationTest_FunctionData_hpp

#include <xdm/ContiguousArray.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/MemoryAdapter.hpp>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace xdm {
  class StructuredArray;
}

namespace xdmGrid {
  class Attribute;
  class Grid;
}

typedef std::pair< double, double > Bounds;

class GridBounds {
private:
  Bounds mBounds[3];
  xdm::DataShape<> mSize;

public:
  GridBounds(
    const Bounds& xbounds,
    const Bounds& ybounds,
    const Bounds& zbounds,
    const xdm::DataShape<>& shape ) :
    mSize( shape ) {
      mBounds[0] = xbounds;
      mBounds[1] = ybounds;
      mBounds[2] = zbounds;
    }
  ~GridBounds() {}

  const Bounds& bounds( int dimension ) const { return mBounds[dimension]; }
  xdm::DataShape<>::size_type size( int dimension ) const {
    return mSize[dimension];
  }

  const xdm::DataShape<>& shape() const { return mSize; }
  xdm::DataShape<>& shape() { return mSize; }

  double nodeCoordinate( int dimension, int index ) const {
    return mBounds[dimension].first +
      index * ( mBounds[dimension].second - mBounds[dimension].first ) /
      mSize[dimension];
  }

  double elementCoordinate( int dimension, int index ) const {
    return 0.5 * (
      nodeCoordinate( dimension, index ) +
      nodeCoordinate( dimension, index + 1 ) );
  }

  void reverseDimensionOrder() {
    std::reverse( mBounds, mBounds + 3 );
    xdm::reverseDimensionOrder( mSize );
  }
};

inline void reverseDimensionOrder( GridBounds& bounds ) {
  bounds.reverseDimensionOrder();
}

inline const GridBounds& testCaseBounds() {
  static GridBounds result(
    std::make_pair( 0.0, 6.28 ),
    std::make_pair( 0.0, 6.28 ),
    std::make_pair( 0.0, 6.28 ),
    xdm::makeShape( 57, 50, 50 ) );
  return result;
}

typedef std::pair<
  xdm::RefPtr< xdmGrid::Grid >,
  xdm::RefPtr< xdmGrid::Attribute >
> ProblemInfo;

ProblemInfo
constructFunctionGrid( const GridBounds& bounds, const std::string& hdfFile );

/// Function object to provide the function to be called in the below
/// FunctionData class.
class Function : public xdm::ReferencedObject {
public:
  virtual ~Function() {}
  virtual double operator()( double x, double y, double z ) = 0;
};

// class that computes the function values given the overall grid bounds and a
// region of interest.
class FunctionData : public xdm::MemoryAdapter {
private:
  GridBounds mGrid;
  xdm::HyperSlab<> mRegionOfInterest;
  std::vector< double > mStorage;
  xdm::RefPtr< xdm::ContiguousArray< double > > mStructuredArray;
  xdm::RefPtr< Function > mFunction;
  xdm::DataShape<> mBlockSize;

public:
  FunctionData(
    const GridBounds& grid,
    const xdm::HyperSlab<>& region,
    xdm::RefPtr< Function > function,
    const xdm::DataShape<>& blockSize = xdm::makeShape( 14, 14, 14 ) );
  virtual ~FunctionData();

  virtual xdm::RefPtr< const xdm::StructuredArray > array() const;
  using xdm::MemoryAdapter::array;

  virtual void writeImplementation( xdm::Dataset* dataset );
  virtual void readImplementation( xdm::Dataset* dataset );
};

/// Constant function returns a constant value over the grid.
class ConstantFunction : public Function {
private:
  double mValue;

public:
  ConstantFunction( double value ) : mValue( value ) {}
  virtual ~ConstantFunction() {}
  virtual double operator()( double, double, double ) { return mValue; }
};

/// Test case computes sin(x*y*z) on the grid.
class TestCaseFunction : public Function {
public:
  virtual ~TestCaseFunction() {}
  virtual double operator()( double x, double y, double z );
};

#endif // xdmIntegrationTest_FunctionData_hpp

