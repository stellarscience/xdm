#ifndef xdm_Dataset_hpp
#define xdm_Dataset_hpp

#include <xdm/Array.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/Slab.hpp>

#include <xdm/NamespaceMacro.hpp>

#include <iostream>

XDM_NAMESPACE_BEGIN

class Dataset;

class InitializeCallback {
public:
  virtual void initializeImplementation(
    Dataset* ds,
    const DataShape<>& shape,
    std::iostream& content ) = 0;
};

class SerializeCallback {
public:
  virtual void serializeImplementation(
    Dataset* ds,
    const Array& data,
    const SlabMap<>& slabMap,
    std::iostream& content ) = 0;
};

class FinalizeCallback {
public:
  virtual void finalizeImplementation( Dataset* ds ) = 0;
};

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

  void setInitializeCallback( InitializeCallback* icb );
  void setSerializeCallback( SerializeCallback* scb );
  void setFinalizeCallback( FinalizeCallback* fcb );

  virtual void initializeImplementation( 
    const DataShape<>& shape, 
    std::iostream& content ) = 0;
  virtual void serializeImplementation( 
    const Array& data, 
    const SlabMap<>& slabMap, 
    std::iostream& content ) = 0;
  virtual void finalizeImplementation() = 0;
private:
  InitializeCallback* mInitializeCallback;
  SerializeCallback* mSerializeCallback;
  FinalizeCallback* mFinalizeCallback;
};

XDM_NAMESPACE_END

#endif // xdm_Dataset_hpp

