#ifndef xdmHdf_HdfDataset_hpp
#define xdmHdf_HdfDataset_hpp

#include <xdm/Dataset.hpp>

#include <memory>
#include <string>

#include <xdmHdf/NamespaceMacro.hpp>

XDM_HDF_NAMESPACE_BEGIN

class HdfDataset : public xdm::Dataset {
public:
  /// Default constructor does not associate with a file.
  HdfDataset();
  /// Constructor takes file, group, and dataset names.
  HdfDataset( 
    const std::string& file, 
    const std::string& group,
    const std::string& dataset );
  virtual ~HdfDataset();

  /// Set the file name.
  void setFile( const std::string& file );
  /// Get the file name.
  const std::string& file() const;

  /// Set the group name.
  void setGroup( const std::string& group );
  /// Get the group name.
  const std::string& group() const;

  /// Set the dataset name.
  void setDataset( const std::string& dataset );
  /// Get the dataset name.
  const std::string& dataset() const;

  //-- Dataset Implementations --//
  virtual const char* format() { return "HDF"; }
  virtual void writeTextContent( xdm::XmlTextContent& text );
  virtual void initializeImplementation(
    xdm::primitiveType::Value type,
    const xdm::DataShape<>& shape );
  virtual void serializeImplementation( 
    const xdm::StructuredArray* data );
  virtual void finalizeImplementation();

private:
  struct Private;
  std::auto_ptr< Private > imp;
};

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_HdfDataset_hpp

