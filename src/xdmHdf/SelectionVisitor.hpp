#ifndef xdmHdf_SelectionVisitor_hpp
#define xdmHdf_SelectionVisitor_hpp

#include <xdm/DataSelectionVisitor.hpp>

#include <hdf5.h>

#include <xdmHdf/NamespaceMacro.hpp>
#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN
  class HyperslabDataSelection;
  class AllDataSelection;
XDM_NAMESPACE_END

XDM_HDF_NAMESPACE_BEGIN

class SelectionVisitor : public xdm::DataSelectionVisitor {
private:
  hid_t mIdent;

public:
  /// Constructor takes the dataspace identifier to act on.
  SelectionVisitor( hid_t ident );
  virtual ~SelectionVisitor();

  //-- Type Safe apply methods from xdm::DataSelectionVisitor --//
  virtual void apply( const xdm::DataSelection& selection );
  virtual void apply( const xdm::AllDataSelection& selection );
  virtual void apply( const xdm::HyperslabDataSelection& selection );
};

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_SelectionVisitor_hpp

