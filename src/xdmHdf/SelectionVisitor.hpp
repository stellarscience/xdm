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
#ifndef xdmHdf_SelectionVisitor_hpp
#define xdmHdf_SelectionVisitor_hpp

#include <xdm/DataSelectionVisitor.hpp>

#include <hdf5.h>

#include <vector>



namespace xdm {
  class AllDataSelection;
  class CoordinateDataSelection;
  class HyperslabDataSelection;
} // namespace xdm

namespace xdmHdf {

class SelectionVisitor : public xdm::DataSelectionVisitor {
private:
  hid_t mIdent;
  
  static std::vector< const hsize_t* > sPointerBuffer; 

public:
  /// Constructor takes the dataspace identifier to act on.
  SelectionVisitor( hid_t ident );
  virtual ~SelectionVisitor();

  //-- Type Safe apply methods from xdm::DataSelectionVisitor --//
  virtual void apply( const xdm::DataSelection& selection );
  virtual void apply( const xdm::AllDataSelection& selection );
  
  // Temporarily removed due to 32/64 bit compatibility issues, and because ther
  // currently no application users of this function.  The implementation needs
  // to be fixed to support copying the input coordinate data when the sizes of
  // types on the system allow for it and do a copy of the coordinate array
  // otherwise.
  // virtual void apply( const xdm::CoordinateDataSelection& selection );
  
  virtual void apply( const xdm::HyperslabDataSelection& selection );
};

} // namespace xdmHdf

#endif // xdmHdf_SelectionVisitor_hpp

