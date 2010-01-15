#ifndef xdm_Dataset_hpp
#define xdm_Dataset_hpp

#include <xdm/StructuredArray.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/PrimitiveType.hpp>
#include <xdm/ReferencedObject.hpp>

#include <xdm/NamespaceMacro.hpp>

#include <iostream>

XDM_NAMESPACE_BEGIN

/// Base class for all dataset access.  A dataset is an interface to writing
/// arrays to disk.  This class should be implemented by inheritors to write an
/// array to disk as simply as possible.  Decorators (callbacks) should be used
/// to customize the data flow for different application environments.
class Dataset : public ReferencedObject {
public:
  Dataset();
  virtual ~Dataset();
  
  //-- Decorators --//

	/// Initialization callback function.  The purpose of this class is to provide
	/// client applications with the ability to decorate the initialization process
	/// of datasets without specific knowledge of the underlying dataset.  An
	/// example of where this is useful is in a parallel application that needs to
	/// communicate all of the relevant data to a certain process in a communicator
	/// to handle the actual file output of the data.
	class InitializeCallback : public ReferencedObject {
	public:
	  virtual void initializeImplementation(
	    Dataset* ds,
      primitiveType::Value type,
	    const DataShape<>& shape,
	    std::iostream& content ) = 0;
	};
	
	/// Serialization callback function.  Decorates the serialization process to
	/// customize data flow.
	class SerializeCallback : public ReferencedObject {
	public:
	  virtual void serializeImplementation(
	    Dataset* ds,
	    const StructuredArray& data,
	    const HyperSlabMap<>& slabMap,
	    std::iostream& content ) = 0;
	};
	
	/// Finalization Callback function.  Decorates the finalization process to
	/// customize the data flow.
	class FinalizeCallback : public ReferencedObject {
	public:
	  virtual void finalizeImplementation( Dataset* ds ) = 0;
	};

  //-- Dataset access functions --//

  /// Initialize a dataset given a shape for the data on disk and initialization
  /// content.
  /// @param shape the shape of the data on disk.
  /// @param content stream with content to initialize the dataset from.
  void initialize( 
    primitiveType::Value type, 
    const DataShape<>& shape, 
    std::iostream& content );

  /// Serialize an array.  Maps a hyper-slab in an array to a hyper-slab on
  /// disk.  Manipulating the stream is optional.
  /// @param data The data to be written.
  /// @param slabMap The mapping from memory space to file space
  /// @param content A stream from which to pull/push content that is generated
  void serialize( 
    const StructuredArray& data, 
    const HyperSlabMap<>& slabMap,
    std::iostream& content );

  /// Complete writing the dataset.
  void finalize();

  /// Set the initialization callback decorator function.  This allows run-time
  /// customization of the initialization data flow.
  void setInitializeCallback( InitializeCallback* icb );
  /// Set the serialization callback decorator function.  This allows run-time
  /// customization of the serialization data flow.
  void setSerializeCallback( SerializeCallback* scb );
  /// Set the finalization callback decorator function.  This allows run-time
  /// customization of the finalization data flow.
  void setFinalizeCallback( FinalizeCallback* fcb );

  //-- Virtual Implementation Functions --//

  /// Pure virtual function to initialize the dataset.  Inheritors should
  /// implement this function to provide the necessary calls required to
  /// initialize a dataset with the given shape from the content provided via
  /// the input stream.  No one should call this except callbacks.
  virtual void initializeImplementation(
    primitiveType::Value type,
    const DataShape<>& shape, 
    std::iostream& content ) = 0;

  /// Pure virtual function to serialize an array.  Inheritors should implement
  /// this function to provide the necessary calls to write an array to disk or
  /// to the input stream, depending on the needs of the underlying dataset
  /// type. No one should call this except for callbacks.
  virtual void serializeImplementation( 
    const StructuredArray& data, 
    const HyperSlabMap<>& slabMap, 
    std::iostream& content ) = 0;

  /// Pure virtual function to complete the process of writing a dataset. This
  /// should be called only by callbacks.
  virtual void finalizeImplementation() = 0;

private: 
  InitializeCallback* mInitializeCallback;
  SerializeCallback* mSerializeCallback;
  FinalizeCallback* mFinalizeCallback;
};

XDM_NAMESPACE_END

#endif // xdm_Dataset_hpp

