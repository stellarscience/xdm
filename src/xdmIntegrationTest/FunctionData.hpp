#ifndef xdmIntegrationTest_FunctionData_hpp
#define xdmIntegrationTest_FunctionData_hpp

#include <xdm/DataShape.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/WritableData.hpp>

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

struct GridBounds {
  Bounds mBounds[3];
  xdm::DataShape<> mSize;
};

inline GridBounds testCaseBounds() {
  GridBounds result;
  result.mBounds[0] = std::make_pair( -2.0, 1.0 );
  result.mBounds[1] = std::make_pair( -1.0, 1.0 );
  result.mBounds[2] = std::make_pair( -1.0, 1.0 );
  result.mSize.setRank( 3 );
  result.mSize[0] = 50;
  result.mSize[1] = 25;
  result.mSize[2] = 25;
  return result;
}

typedef std::pair< 
  xdm::RefPtr< xdmGrid::Grid >, 
  xdm::RefPtr< xdmGrid::Attribute >
> ProblemInfo;

ProblemInfo
constructFunctionGrid( const GridBounds& bounds, const std::string& hdfFile );

// class that computes the function values given the overall grid bounds and a
// region of interest.
class FunctionData : public xdm::WritableData {
private:
  GridBounds mGrid;
  xdm::HyperSlab<> mRegionOfInterest;
  std::vector< double > mStorage;
  xdm::RefPtr< xdm::StructuredArray > mStructuredArray;

public:
  FunctionData( const GridBounds& grid, const xdm::HyperSlab<>& region );
  virtual ~FunctionData();

  virtual void writeImplementation( xdm::Dataset* dataset );

protected:
  virtual double evaluate( int i, int j, int k ) = 0;
};

/// FunctionData implementation that returns a constant value over the region of
/// interest.
class ConstantFunction : public FunctionData {
private:
  double mValue;

public:
  ConstantFunction( 
    const GridBounds& grid,
    const xdm::HyperSlab<>& region, 
    double value ) :
    FunctionData( grid, region ),
    mValue( value ) {
  }
  virtual ~ConstantFunction() {}

protected:
  virtual double evaluate( int, int, int ) { return mValue; }
};

#endif // xdmIntegrationTest_FunctionData_hpp

