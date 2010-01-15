//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.      
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
#ifndef xdm_HyperslabDataSelection_hpp
#define xdm_HyperslabDataSelection_hpp

#include <xdm/DataSelection.hpp>
#include <xdm/DataSelectionVisitor.hpp>
#include <xdm/HyperSlab.hpp>

XDM_NAMESPACE_BEGIN

/// DataSelection consisting sampling data with a given start, stride, and
/// count.
class HyperslabDataSelection : public DataSelection {
private:
  HyperSlab<> mHyperslab;
public:
  /// Constructor takes a hyperslab to select.  Defaults to an empty hyperslab.
  HyperslabDataSelection( const HyperSlab<>& hyperslab = HyperSlab<>() ) :
    mHyperslab( hyperslab ) {}

  virtual ~HyperslabDataSelection() {}

  /// Set the hyperslab for the selection.
  void setHyperslab( const HyperSlab<>& hyperslab ) { 
    mHyperslab = hyperslab; 
  }
  /// Get the selection's hyperslab.
  const HyperSlab<>& hyperslab() const {
    return mHyperslab;
  }

  /// Visitor accept interface.
  virtual void accept( DataSelectionVisitor& v ) const {
    v.apply( *this );
  }
};

inline bool operator==( 
  const HyperslabDataSelection& lhs, 
  const HyperslabDataSelection& rhs ) {
  return ( lhs.hyperslab() == rhs.hyperslab() );
}

XDM_NAMESPACE_END

#endif // xdm_HyperslabDataSelection_hpp

