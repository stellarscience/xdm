#include <xdmHdf/SelectionVisitor.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/CoordinateDataSelection.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/ThrowMacro.hpp>

#include <stdexcept>

XDM_HDF_NAMESPACE_BEGIN

SelectionVisitor::SelectionVisitor( hid_t ident ) :
  mIdent( ident ) {
}

SelectionVisitor::~SelectionVisitor() {
}

void SelectionVisitor::apply( const xdm::DataSelection& selection ) {
  XDM_THROW( std::runtime_error( "Unknown data selection type" ) );
}

void SelectionVisitor::apply( const xdm::AllDataSelection& selection ) {
  H5Sselect_all( mIdent );
}

void SelectionVisitor::apply( const xdm::CoordinateDataSelection& selection ) {
  xdm::CoordinateArray<> coords = selection.coordinates();
  H5Sselect_elements( 
    mIdent, 
    H5S_SELECT_SET, 
    coords.numberOfElements(), 
    (hsize_t*)(coords.values()) );
}

void SelectionVisitor::apply( const xdm::HyperslabDataSelection& selection ) {
  xdm::HyperSlab< hsize_t > slab( selection.hyperslab() );
  H5Sselect_hyperslab( 
    mIdent,
    H5S_SELECT_SET, 
    &(slab.start( 0 )), 
    &(slab.stride( 0 )),
    &(slab.count( 0 )),
    NULL );
}

XDM_HDF_NAMESPACE_END

