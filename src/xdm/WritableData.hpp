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
#ifndef xdm_WritableData_hpp
#define xdm_WritableData_hpp

#include <xdm/ReferencedObject.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class Dataset;

/// Definition of the interface for writing data to a Dataset.  Implementations
/// of this interface define the data to be written to the Dataset.  Writable
/// can be tagged as dynamic if it evolves over time.  Dynamic data will invoke
/// a write to the dataset upon every call to write().  If the data is not
/// dynamic, then the data can be set as requiring an update if a new write is
/// required.
class WritableData : public xdm::ReferencedObject {
public:
  WritableData( bool isDynamic = false );
  virtual ~WritableData();

  /// Determine if the data changes as time evolves.  Guarantees that data will
  /// be written every time a write is requested.
  bool isDynamic() const;
  /// Set whether or not the data changes as time evolves.
  void setIsDynamic( bool isDynamic );

  /// Determine if the data needs an update.
  bool needsUpdate() const;
  /// Specify that the data needs an update.
  void setNeedsUpdate( bool needsUpdate );

  /// Write the data to the specified dataset.
  void write( Dataset* dataset ); 

private:
  /// Method to be implemented by inheritors to define the data to be written to
  /// the dataset.
  virtual void writeImplementation( Dataset* dataset ) = 0;

  bool mIsDynamic;
  bool mNeedsUpdate;
};

XDM_NAMESPACE_END

#endif // xdm_WritableData_hpp

