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
#ifndef xdm_ArrayAdapter_hpp
#define xdm_ArrayAdapter_hpp

#include <xdm/DataSelectionMap.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/MemoryAdapter.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class Dataset;
class StructuredArray;

/// Writes an Array with a given selection to a selection within a dataset.
class ArrayAdapter : public MemoryAdapter {
public:
  ArrayAdapter( RefPtr< StructuredArray > array, bool isDynamic = false );
  virtual ~ArrayAdapter();

  RefPtr< StructuredArray > array();
  RefPtr< const StructuredArray > array() const;
  void setArray( RefPtr< StructuredArray > array );

  const DataSelectionMap& selectionMap() const;
  void setSelectionMap( const DataSelectionMap& selectionMap );

  virtual void writeImplementation( Dataset* dataset );

private:
  RefPtr< StructuredArray > mArray;
  DataSelectionMap mSelectionMap;
};

XDM_NAMESPACE_END

#endif // xdm_ArrayAdapter_hpp

