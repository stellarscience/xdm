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
#ifndef xdm_AllDataSelection_hpp
#define xdm_AllDataSelection_hpp

#include <xdm/DataSelection.hpp>
#include <xdm/DataSelectionVisitor.hpp>



namespace xdm {

/// DataSelection that simply selects all data.
class AllDataSelection : public DataSelection {
public:
  AllDataSelection() {}
  virtual ~AllDataSelection() {}
  virtual void accept( DataSelectionVisitor& v ) const { v.apply(*this); }
};

inline bool operator==( 
  const AllDataSelection& lhs, 
  const AllDataSelection& rhs ) {
  return true;
}

} // namespace xdm

#endif // xdm_AllDataSelection_hpp

