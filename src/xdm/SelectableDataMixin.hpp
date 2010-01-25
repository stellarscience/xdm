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
#ifndef xdm_SelectableDataMixin_hpp
#define xdm_SelectableDataMixin_hpp

#include <xdm/RefPtr.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class AllDataSelection;
class DataSelection;

/// Mixin that provides data selection services to inheritors.  Classes that
/// require the ability to select data should inherit this interface and
/// implementation.
class SelectableDataMixin {
public:

  /// Set the selection for the data.
  void setSelection( RefPtr< DataSelection > selection );
  /// Get the const data selection.
  RefPtr< const DataSelection > selection() const;
  /// Get the data selection.
  RefPtr< DataSelection > selection();

  /// Convenience method to select an entire dataset.
  void selectAll();
  
protected:
  
  /// Default constructor defaults to selecting all data.
  SelectableDataMixin();
  ~SelectableDataMixin();

private:
  RefPtr< DataSelection > mSelection;
  static RefPtr< AllDataSelection > sAllSelection;
};

XDM_NAMESPACE_END

#endif // xdm_SelectableDataMixin_hpp

