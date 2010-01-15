#ifndef xdm_Dataset_hpp
#define xdm_Dataset_hpp

#include <xdm/Array.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/Slab.hpp>

#include <xdm/NamespaceMacro.hpp>

#include <iostream>

XDM_NAMESPACE_BEGIN

class Dataset {
public:
  Dataset();
  virtual ~Dataset();

  void initialize( const DataShape<>& shape, std::iostream& content );
  void serialize( 
    const Array& data, 
    const SlabMap<>& slabMap,
    std::iostream& content );
  void finalize();

  virtual void initializeImplementation( 
    const DataShape<>& shape, 
    std::iostream& content ) = 0;
  virtual void serializeImplementation( 
    const Array& data, 
    const SlabMap<>& slabMap, 
    std::iostream& content ) = 0;
  virtual void finalizeImplementation() = 0;
};

XDM_NAMESPACE_END

#endif // xdm_Dataset_hpp

