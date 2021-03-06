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
#ifndef xdm_DataSelectionMap_hpp
#define xdm_DataSelectionMap_hpp

#include <xdm/RefPtr.hpp>



namespace xdm {

class DataSelection;

/// Definition of a mapping from one selection to another.
class DataSelectionMap {
private:
  RefPtr< DataSelection > mDomain;
  RefPtr< DataSelection > mRange;

public:
  /// Default constructor selects all points in both domain and range spaces.
  DataSelectionMap();
  /// Initialize the domain and range selections.
  DataSelectionMap(
    RefPtr< DataSelection > domain,
    RefPtr< DataSelection > range );
  ~DataSelectionMap();

  RefPtr< const DataSelection > domain() const;
  void setDomain( RefPtr< DataSelection > domain );

  RefPtr< const DataSelection > range() const;
  void setRange( RefPtr< DataSelection > range );
};

} // namespace xdm

#endif // xdm_DataSelectionMap_hpp
