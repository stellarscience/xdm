#ifndef xdm_CompositeData_hpp
#define xdm_CompositeData_hpp

#include <xdm/Data.hpp>
#include <xdm/RefPtr.hpp>

#include <vector>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Class that groups multiple Data nodes together.  CompositeData is a Data
/// type that can hold child Data types.  It is used to construct a large
/// dataset from multiple smaller datasets.
class CompositeData : public Data {
public:
  CompositeData();
  virtual ~CompositeData();

  /// Add a child Data element.
  virtual void appendChild( Data* child );
  
  /// Get the const child item at the numbered location.
  const Data* child( unsigned int i ) const;
  /// Get the child item at the numbered location.
  Data* child( unsigned int i );

private:
  std::vector< RefPtr< Data > > mChildData;
};

XDM_NAMESPACE_END

#endif // xdm_CompositeData_hpp

