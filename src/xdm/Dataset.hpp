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

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

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
};

XDM_NAMESPACE_END

#endif // xdm_Dataset_hpp

