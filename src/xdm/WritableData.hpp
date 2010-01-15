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

