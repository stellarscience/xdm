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
/// initializing and reading or writing.
class BasicDatasetUpdateCallback : public ReferencedObject {
public:
  virtual void update( Dataset* dataset ) = 0;
};

/// UpdateCallback subclass that uses the input template argument to pass a
/// concrete subclass of Dataset into the update call.  This can be used to
/// define a type safe callback that will apply only to a concrete subclass of
/// Dataset.
template< typename T >
class DatasetUpdateCallback : public BasicDatasetUpdateCallback {
public:
  void update( Dataset* dataset ) {
    T* typedDataset = dynamic_cast< T* >( dataset );
    if ( typedDataset ) {
      this->update( typedDataset );
    } else {
      XDM_THROW( std::runtime_error( "Invalid Dataset type for callback." ) );
    }
  }

  virtual void update( T* dataset ) = 0;
};

/// Base class for all dataset access.
///
/// This class controls the writing of arrays to disk.  Subclasses should
/// implement the process of writing for the database type they support.  In
/// addition, further subclassing of this object can accomplish further
/// customization in the data writing process, for example a proxy object that
/// uses MPI to pass data between processes.
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
  
  /// Virtual method to invoke the Dataset's update callback (if it has one).
  virtual void update();
  
  //-- Dataset metadata functions --//

  /// Pure virtual function to return a string that contains the string
  /// identifiying the dataset's format for identification in XML metadata.
  virtual const char* format() = 0;

  /// Pure virtual function to write the dataset's text content (the light
  /// portion of the dataset).
  virtual void writeTextContent( XmlTextContent& text ) = 0;
  
  //-- Dataset access functions --//

  /// Initialize a dataset given a shape for the data on disk and initialization
  /// content.
  /// @param shape the shape of the data on disk.
  /// @param content stream with content to initialize the dataset from.
  void initialize( primitiveType::Value type, const DataShape<>& shape );

  /// Serialize an array.  Maps a hyper-slab in an array to a hyper-slab on
  /// disk.  Manipulating the stream is optional.
  /// @param data The data to be written.
  /// @param slabMap The mapping from memory space to file space
  /// @param content A stream from which to pull/push content that is generated
  void serialize( const StructuredArray* data, 
    const DataSelectionMap& selectionMap );

  /// Complete writing the dataset.
  void finalize();

protected:
  //-- Virtual Implementation Functions --//

  /// Pure virtual function to initialize the dataset.  Inheritors should
  /// implement this function to provide the necessary calls required to
  /// initialize a dataset with the given shape from the content provided via
  /// the input stream.  No one should call this except callbacks.
  virtual void initializeImplementation( primitiveType::Value type, 
    const DataShape<>& shape ) = 0;

  /// Pure virtual function to serialize an array.  Inheritors should implement
  /// this function to provide the necessary calls to write an array to disk or
  /// to the input stream, depending on the needs of the underlying dataset
  /// type. No one should call this except for callbacks.
  virtual void serializeImplementation( const StructuredArray* data,
    const DataSelectionMap& selectionMap ) = 0;

  /// Pure virtual function to complete the process of writing a dataset. This
  /// should be called only by callbacks.
  virtual void finalizeImplementation() = 0;

private:
  RefPtr< BasicDatasetUpdateCallback > mUpdateCallback;
};

XDM_NAMESPACE_END

#endif // xdm_Dataset_hpp

