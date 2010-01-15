#ifndef xdm_Array_hpp
#define xdm_Array_hpp

#include <xdm/DataShape.hpp>

#include <xdm/NamespaceMacro.hpp>

#include <vector>

XDM_NAMESPACE_BEGIN

class Array {
public:
  explicit Array( const DataShape<>& shape );
  ~Array();

  size_t size() const;

  float& at( size_t i );
  const float& at( size_t i ) const;

  float* data();
  const float* data() const;

  const DataShape<>& shape() const {  
    return mShape;
  }

private:
  std::vector< float > mData;
  DataShape<> mShape;
};

XDM_NAMESPACE_END

#endif // xdm_Array_hpp

