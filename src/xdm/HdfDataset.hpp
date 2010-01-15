#ifndef xdm_HdfDataset_hpp
#define xdm_HdfDataset_hpp

#include <xdm/Dataset.hpp>

#include <memory>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class HdfDataset : public Dataset {
public:
  HdfDataset();
  virtual ~HdfDataset();

  //-- Dataset Implementations --//
  virtual void initializeImplementation( 
    const DataShape<>& shape, 
    std::iostream& content );
  virtual void serializeImplementation(
    const StructuredArray& data,
    const HyperSlabMap<>& memory_map,
    std::iostream& content );
  virtual void finalizeImplementation();

private:
  struct Private;
  std::auto_ptr< Private > imp;
};

XDM_NAMESPACE_END

#endif // xdm_HdfDataset_hpp

