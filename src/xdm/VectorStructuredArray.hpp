//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_VectorStructuredArray_hpp
#define xdm_VectorStructuredArray_hpp

#include <xdm/TemplateStructuredArray.hpp>

#include <vector>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// StructuredArray that owns it's own storage and manages it with a standard
/// vector.
template< typename T >
class VectorStructuredArray : public TemplateStructuredArray< T > {
private:
  std::vector< T > mVector;
public:
  VectorStructuredArray( size_t size ) :
    TemplateStructuredArray<T>( 0, xdm::makeShape( size ) ),
    mVector( size ) 
  {
    setData( &mVector[0] );
  }
  virtual ~VectorStructuredArray() {}
};

XDM_NAMESPACE_END

#endif // xdm_VectorStructuredArray_hpp

