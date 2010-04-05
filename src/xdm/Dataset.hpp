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

// Code Review Matter (open): #include order
// Is there an #include order dependence? Could these be arranged
// in alphabetical order?
// -- K. R. Walker on 2010-01-19

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

      // Code Review Matter (open): XDM_THROW disabled?
      // How does a client check for an error condition if XDM_THROW
      // is disabled? Did you consider a system similar to errno?
      // -- K. R. Walker on 2010-01-19

      XDM_THROW( std::runtime_error( "Invalid Dataset type for callback." ) );
    }
  }

  /// Update the specific subclass of Dataset that the callback is designed to
  /// operate on.
  virtual void update( T* dataset ) = 0;
};

// Code Review Matter (open): Database Definition
// Did you consider defining "database" in the context of the 
// Dataset documentation?
// -- K. R. Walker on 2010-01-19

// Code Review Matter (open): Pure Virtual
// The class is described as a virtual base class, but there are pure virtual
// member functions. Should the documentation be updated to reflect this, or
// is the distinction so minor that it doesn't need to be called out?
// -- K. R. Walker on 2010-01-19

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
  
  /// Enumeration of values to determine how to open the dataset.
  enum InitializeMode {
    kInvalid = 0,
    kRead,
    kCreate,
    kModify
  };

public:
  Dataset();
  virtual ~Dataset();

  // Code Review Matter (open): updateCallback() return constness
  // Are there invariants within BasicDatasetUpdateCallback that must
  // protected (via constness) when the Dataset is const. Could a client
  // conceivably wish to modify the update callback independently of the
  // dataset?
  // -- K. R. Walker on 2010-01-19

  /// Get the update callback that will be executed at update time.
  RefPtr< BasicDatasetUpdateCallback > updateCallback();
  /// Get the const update callback that will be executed at update time.
  RefPtr< const BasicDatasetUpdateCallback > updateCallback() const;
  /// Set the callback to be executed at dataset update time.
  void setUpdateCallback( RefPtr< BasicDatasetUpdateCallback > callback );
  
  /// Invoke the update callback for the dataset.
  /// @see BasicDatasetUpdateCallback
  virtual void update();

  // Determine if the dataset has been initialized.
  bool isInitialized() const;
  // Determine the data shape.
  const DataShape<>& shape() const;
  
  // Code Review Matter (open): Doxygen groups
  // Did you consider using Doxygen documentation groups to aid in the
  // grouping of member functions?
  // -- K. R. Walker on 2010-01-19

  //-- Dataset metadata functions --//

  // Code Review Matter (open): C-string
  // Did you consider using a std::string as the return value for format()?
  // -- K. R. Walker on 2010-01-19

  /// Return a string that identifies the database type used by the dataset.
  virtual const char* format() = 0;

  /// Write any text required to locate the dataset.
  virtual void writeTextContent( XmlTextContent& text ) = 0;
  
  //-- Dataset access functions --//

  // Code Review Matter (open): initialize
  // What does it mean to 'initialize' a dataset? Is a dataset "opened"
  // and prepared for writing? Is there more elaborate
  // documentation available elsewhere for the following member functions?
  // -- K. R. Walker on 2010-01-19

  // Code Review Matter (open): pre/post-conditions
  // Did you consider adding preconditions and postconditions to the
  // member functions?
  // -- K. R. Walker on 2010-01-19

  // Code Review Matter (open): namespaced enum standard
  // Stellar doesn't have an official standard for namespaced enumerations,
  // though de-facto standard appears to be similar to:
  //
  // namespace Primitive {
  //   enum Type {
  //     kInvalid = 0,
  //     kInt,
  //     kDouble
  // } // namespace Primitive
  //
  // This would make your initialize interface appear as:
  //   void initialize( Primitive::Type type, ... );
  //
  // Should this be added to our software standards?
  // -- K. R. Walker on 2010-01-19

  // Code Review Matter (open): const 'type'
  // Does the implementation ever change 'type'? Did you consider 
  // making 'type' const?
  // -- K. R. Walker on 2010-01-19
  
  // Code Review Matter (open): Strategy pattern.
  // Currently the public methods are simple pass throughs to the protected
  // virtual implementation methods. Did you consider moving logic that can be
  // shared among all subclasses (such as validating inputs) to the base class
  // for reuse?
  // Will Dicharry 2010-01-19

  /// Initialize a dataset.  Calls the protected virtual
  /// initializeImplementation to provide a point of customization.
  /// @param type Type information for the data to be written to disk.
  /// @param shape the shape of the data on disk.
  /// @param mode Choose to read, create, or modify.
  ///
  /// @throw DatasetNotFound A dataset opened for reading could not be found.
  /// @throw DataspaceMismatch The requested space and the space on disk differ.
  /// @throw DatatypeMismatch The requested type and the type on disk differ.
  ///
  /// @return The shape of the dataset on disk.
  DataShape<> initialize( 
    primitiveType::Value type, 
    const DataShape<>& shape, 
    const InitializeMode& mode );

  // Code Review Matter (open): serialize vs serializeImplementation
  // The documentation for serialize and serializeImplemenation appear to
  // contradict each other. serialize says it does the mapping while
  // serializeImplementation does the writing, but serializeImplementation
  // says that implementors should provide a mapping. 
  // Did you consider consolidating much of this documentatation in the
  // class description while giving the members very brief summaries and
  // referring to the class?
  // -- K. R. Walker on 2010-01-19

  /// Serialize an array to disk.  Maps a subset of the input array to a subset
  /// of the output space on disk.  Uses the virtual protected member
  /// serializeImplementation to defer the process of writing the data to
  /// subclasses.
  /// @param data The array to be written to disk.
  /// @param selectionMap A map that specifies the subset of the data in memory
  /// and on disk.
  ///
  /// @throw DataSizeMismatch The number of selected elements in the array and
  /// the number of selected elements in the dataset differ.
  void serialize( const StructuredArray* data, 
    const DataSelectionMap& selectionMap );

  /// Deserialize an array from disk. Maps a subset of the dataset on disk to a
  /// subset of the output array passed in.  Uses the virtual protected member
  /// serializeImplementation to defer the process of writing the data to
  /// subclasses.
  /// @param data An already allocated array to hold the data.
  /// @param selectionMap A map that specifies the subset of the data on disk to
  /// map to the array in memory.
  ///
  /// @throw DataSizeMismatch The number of selected elements in the array and
  /// the number of selected elements in the dataset differ.
  void deserialize( StructuredArray* data, 
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
  virtual DataShape<> initializeImplementation( 
    primitiveType::Value type, 
    const DataShape<>& shape,
    const InitializeMode& mode ) = 0;

  /// Implementation method to define array serialization. Inheritors should
  /// implement this method to provide the necessary calls for mapping a subset
  /// of the input array to a subset of the dataset on disk. 
  virtual void serializeImplementation( 
    const StructuredArray* data,
    const DataSelectionMap& selectionMap ) = 0;

  /// Implementation method to define array deserialization. Inheritors should
  /// implement this method to provide the necessary calls for mapping a subset
  /// of a dataset on disk to the specified, allocated output array.
  virtual void deserializeImplementation(
    StructuredArray* data,
    const DataSelectionMap& selectionMap ) = 0;

  /// Definition of the finalization process for a dataset.  Inheritors should
  /// implement this method to provide the necessary calls for completing the
  /// write of a dataset.
  virtual void finalizeImplementation() = 0;

private:
  RefPtr< BasicDatasetUpdateCallback > mUpdateCallback;
  bool mIsInitialized;
  DataShape<> mShape;
};

XDM_NAMESPACE_END

#endif // xdm_Dataset_hpp

