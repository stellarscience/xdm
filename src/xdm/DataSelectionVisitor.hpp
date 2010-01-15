#ifndef xdm_DataSelectionVisitor_hpp
#define xdm_DataSelectionVisitor_hpp

#include <xdm/DataSelection.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class AllDataSelection;
class HyperslabDataSelection;

class DataSelectionVisitor : public virtual ReferencedObject {
public:
  DataSelectionVisitor();
  virtual ~DataSelectionVisitor();

  //-- apply methods for known subclasses of DataSelection --//
  virtual void apply( const DataSelection& selection );
  virtual void apply( const AllDataSelection& selection );
  virtual void apply( const HyperslabDataSelection& selection );
};

XDM_NAMESPACE_END

#endif // xdm_DataSelectionVisitor_hpp

