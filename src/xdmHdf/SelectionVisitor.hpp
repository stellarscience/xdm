#ifndef xdmHdf_SelectionVisitor_hpp
#define xdmHdf_SelectionVisitor_hpp

#include <xdm/DataSelectionVisitor.hpp>

#include <hdf5.h>

#include <vector>

#include <xdmHdf/NamespaceMacro.hpp>
#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN
  class AllDataSelection;
  class CoordinateDataSelection;
  class HyperslabDataSelection;
XDM_NAMESPACE_END

XDM_HDF_NAMESPACE_BEGIN

class SelectionVisitor : public xdm::DataSelectionVisitor {
private:
  hid_t mIdent;
  
  static std::vector< const hsize_t* > sPointerBuffer; 

public:
  /// Constructor takes the dataspace identifier to act on.
  SelectionVisitor( hid_t ident );
  virtual ~SelectionVisitor();

  //-- Type Safe apply methods from xdm::DataSelectionVisitor --//
  virtual void apply( const xdm::DataSelection& selection );
  virtual void apply( const xdm::AllDataSelection& selection );
  
  // Temporarily removed due to 32/64 bit compatibility issues, and because ther
  // currently no application users of this function.  The implementation needs
  // to be fixed to support copying the input coordinate data when the sizes of
  // types on the system allow for it and do a copy of the coordinate array
  // otherwise.
  // virtual void apply( const xdm::CoordinateDataSelection& selection );
  
  virtual void apply( const xdm::HyperslabDataSelection& selection );
};

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_SelectionVisitor_hpp

