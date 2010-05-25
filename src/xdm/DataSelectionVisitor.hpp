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
#ifndef xdm_DataSelectionVisitor_hpp
#define xdm_DataSelectionVisitor_hpp

#include <xdm/DataSelection.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>



namespace xdm {

class AllDataSelection;
class CoordinateDataSelection;
class HyperslabDataSelection;

class DataSelectionVisitor : public virtual ReferencedObject {
public:
  DataSelectionVisitor();
  virtual ~DataSelectionVisitor();

  //-- apply methods for known subclasses of DataSelection --//
  virtual void apply( const DataSelection& selection );
  virtual void apply( const AllDataSelection& selection );
  virtual void apply( const HyperslabDataSelection& selection );
  virtual void apply( const CoordinateDataSelection& selection );

};

} // namespace xdm

#endif // xdm_DataSelectionVisitor_hpp

