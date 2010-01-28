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
  const SizeT* mValues;
  SizeT mRank;
  SizeT mNumberOfElements;

public:
  
  typedef SizeT size_type;

  CoordinateArray() :
    mValues( 0 ),
    mRank( 0 ),
    mNumberOfElements( 0 ) {}

  /// Construct given a one dimensional array of interleaved coordinate values.
  /// The array is expected to be of size at least numberOfElements*rank.
  /// @param values Array with interleaved coordinate values.
  /// @param rank Number of values required to identify a point.
  /// @param numberOfElements Number of points to select.
  CoordinateArray( 
    const size_type* values, 
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
    return *this;
  }

  const size_type* values() const { return mValues; }
  size_type* values() { return mValues; }
  size_type rank() const { return mRank; }
  size_type numberOfElements() const { return mNumberOfElements; }
};

/// Coordinate value data selection class.  The elements of a selection will be
/// assigned to the target space at location provided by the coordinates in the
/// order specified in an array.
class CoordinateDataSelection : public DataSelection {
public:
  CoordinateDataSelection();
  explicit CoordinateDataSelection( const CoordinateArray<>& coordinates );
  virtual ~CoordinateDataSelection();

  const CoordinateArray<>& coordinates() const;
  void setCoordinates( const CoordinateArray<>& coordinates );

  virtual void accept( DataSelectionVisitor& v ) const;
private:
  CoordinateArray<> mCoordinates;
};

XDM_NAMESPACE_END

#endif // xdm_CoordinateDataSelection_hpp

