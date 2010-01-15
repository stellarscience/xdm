#ifndef xdmGrid_Polyvertex_hpp
#define xdmGrid_Polyvertex_hpp

#include <xdmGrid/UnstructuredTopology.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Polyvertex : public xdmGrid::UnstructuredTopology {
public:
  Polyvertex();
  virtual ~Polyvertex();

  XDM_META_ITEM( "Polyvertex" );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Polyvertex_hpp
