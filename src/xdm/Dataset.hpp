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
#ifndef xdm_Dataset_hpp
#define xdm_Dataset_hpp

#include <xdm/StructuredArray.hpp>
#include <xdm/DataSelectionMap.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/PrimitiveType.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/SelectableDataMixin.hpp>
#include <xdm/XmlTextContent.hpp>

#include <stdexcept>

#include <xdm/NamespaceMacro.hpp>
#include <xdm/ThrowMacro.hpp>

XDM_NAMESPACE_BEGIN

class Dataset;

/// Callback class for updating a dataset before the operations of
/// initializing and reading or writing.  This callback has no knowledge of the
/// underlying subclass of dataset that it is operating on, it receives a
/// generic Dataset.  For a callback with knowledge of specific Dataset
/// subclasses, see DatasetUpdateCallback.
/// @see DatasetUpdateCallback
class BasicDatasetUpdateCallback : public ReferencedObject {
public:
  /// Update a dataset.  This method provides a customization point for library
  /// clients to specify custom behavior when updating a dataset.
  virtual void update( Dataset* dataset ) = 0;
};

/// UpdateCallback subclass that uses the input template argument to pass a
/// concrete subclass of Dataset into the update call.  This can be used to
/// define a type safe callback that will apply only to a concrete subclass of
/// Dataset.
///
/// @param T The subclass of Dataset that the callback should operate on.
template< typename T >
class DatasetUpdateCallback : public BasicDatasetUpdateCallback {
public:
  /// Update the visitor if it is a subclass of the template parameter,
  /// otherwise raise an error.
  /// @throw std::runtime_error If the input dataset is not a subclass of T.
  // Code Review Matter (open): Typed exceptions. Errors should be identifiable
  // from the type of exception thrown, not the content of the exception. In the
  // unmerged branch that implements dataset input, exceptions are broken into
  // an identifiable hierarchy.
  // -- Will Dicharry 2010-01-19
  void update( Dataset* dataset ) {
    T* typedDataset = dynamic_cast< T* >( dataset );
    if ( typedDataset ) {
      this->update( typedDataset );
    } else {
      XDM_THROW( std::runtime_error( "Invalid Dataset type for callback." ) );
    }
  }

  /// Update the specific subclass of Dataset that the callback is designed to
  /// operate on.
  virtual void update( T* dataset ) = 0;
};

/// @brief Interface for writing array data to disk.
///
/// This class provides the interface for defining services that write array
/// based data to disk.  The interface provides methods for initializing,
/// writing, and finalizing data.  The interface should be subclassed to define
/// new databases for arrays.
///
/// This is a virtual base class. it is left to subclasses to define the virtual
/// protected members that define initialization, serialization and
/// finalization.  The public interface of the class provides non-virtual
/// methods that call the protected virtual members.
class Dataset : public ReferencedObject {
public:
  Dataset();
  virtual ~Dataset();

  /// Get the update callback that will be executed at update time.
  BasicDatasetUpdateCallback* updateCallback();
  /// Get the const update callback that will be executed at update time.
  const BasicDatasetUpdateCallback* updateCallback() const;
  /// Set the callback to be executed at dataset update time.
  void setUpdateCallback( BasicDatasetUpdateCallback* callback );
  
  /// Invoke the update callback for the dataset.
  /// @see BasicDatasetUpdateCallback
  virtual void update();
  
  //-- Dataset metadata functions --//

  /// Return a string that identifies the database type used by the dataset.
  virtual const char* format() = 0;

  /// Write any text required to locate the dataset.
  virtual void writeTextContent( XmlTextContent& text ) = 0;
  
  //-- Dataset access functions --//

  /// Initialize a dataset.  Calls the protected virtual
  /// initializeImplementation to provide a point of customization.
  /// @param type Type information for the data to be written to disk.
  /// @param shape the shape of the data on disk.
  // Code Review Matter (open): Strategy pattern.
  // Currently the public methods are simple pass throughs to the protected
  // virtual implementation methods. Did you consider moving logic that can be
  // shared among all subclasses (such as validating inputs) to the base class
  // for reuse?
  // Will Dicharry 2010-01-19
  void initialize( primitiveType::Value type, const DataShape<>& shape );

  /// Serialize an array to disk.  Maps a subset of the input array to a subset
  /// of the output space on disk.  Uses the virtual protected member
  /// serializeImplementation to defer the process of writing the data to
  /// subclasses.
  /// @param data The array to be written to disk.
  /// @param selectionMap A map that specifies the subset of the data in memory
  /// and on disk.
  void serialize( const StructuredArray* data, 
    const DataSelectionMap& selectionMap );

  /// Complete the process of writing a dataset.  Calls the protected virtual
  /// finalizeImplementation to defer the process of completing the write to
  /// subclasses.
  void finalize();

protected:
  //-- Virtual Implementation Functions --//

  /// Implementation method to define how a dataset is to be initialized.
  /// Inheritors should implement this function to provide the necessary calls
  /// required to initialize a dataset with the given type and shape.
  virtual void initializeImplementation( primitiveType::Value type, 
    const DataShape<>& shape ) = 0;

  /// Implementation method to define array serialization. Inheritors should
  /// implement this method to provide the necessary calls for mapping a subset
  /// of the input array to a subset of the dataset on disk. 
  virtual void serializeImplementation( 
    const StructuredArray* data,
    const DataSelectionMap& selectionMap ) = 0;

  /// Definition of the finalization process for a dataset.  Inheritors should
  /// implement this method to provide the necessary calls for completing the
  /// write of a dataset.
  virtual void finalizeImplementation() = 0;

private:
  RefPtr< BasicDatasetUpdateCallback > mUpdateCallback;
};

XDM_NAMESPACE_END

#endif // xdm_Dataset_hpp

