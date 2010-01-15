#ifndef xdm_CoordinateDataSelection_hpp
#define xdm_CoordinateDataSelection_hpp

#include <xdm/DataSelection.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Class holds the coordinate values for a coordinate selection in a
/// two-dimensional array.
template< typename SizeT = size_t >
class CoordinateArray {
private:
  const SizeT** mValues;
  SizeT mRank;
  SizeT mNumberOfElements;

public:
  
  typedef SizeT size_type;

  CoordinateArray() :
    mValues( 0 ),
    mRank( 0 ),
    mNumberOfElements( 0 ) {}

  /// construct given a two-dimensional array and it's sizes.  Does not take
  /// ownership.
  CoordinateArray( 
    const size_type** values, 
    size_type rank, 
    size_type numberOfElements ) :
    mValues( values ),
    mRank( rank ),
    mNumberOfElements( numberOfElements ) {}
  /// Shallow copy, shares data with the other array.
  CoordinateArray( const CoordinateArray& other ) :
    mValues( other.mValues ),
    mRank( other.mRank ),
    mNumberOfElements( other.mNumberOfElements ) {}
  ~CoordinateArray() {}

  /// Shallow copy assignment operator (assigned and assignee share data).
  CoordinateArray& operator=( const CoordinateArray& rhs ) {
    if ( &rhs == this ) { return *this; }
    mValues = rhs.mValues;
    mRank = rhs.mRank;
    mNumberOfElements = rhs.mNumberOfElements;
  }

  const size_type** values() const { return mValues; }
  size_type rank() const { return mRank; }
  size_type numberOfElements() const { return mNumberOfElements; }
};

/// Coordinate value data selection class.  The elements of a selection will be
/// assigned to the target space at location provided by the coordinates in the
/// order specified in an array.
class CoordinateDataSelection : public DataSelection {
public:
  CoordinateDataSelection();
  CoordinateDataSelection( const CoordinateArray<>& coordinates );
  virtual ~CoordinateDataSelection();

  const CoordinateArray<>& coordinates() const;
  void setCoordinates( const CoordinateArray<>& coordinates );

  virtual void accept( DataSelectionVisitor& v ) const;
private:
  CoordinateArray<> mCoordinates;
};

XDM_NAMESPACE_END

#endif // xdm_CoordinateDataSelection_hpp

