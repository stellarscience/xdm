//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmHdf/SelectionVisitor.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/CoordinateDataSelection.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/ThrowMacro.hpp>

#include <stdexcept>

#include <cassert>

XDM_HDF_NAMESPACE_BEGIN

std::vector< const hsize_t* > SelectionVisitor::sPointerBuffer;

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

/*
// Temporarily removed due to 32-64 bit compatibility issues.  See the note in
// the class declaration.
void SelectionVisitor::apply( const xdm::CoordinateDataSelection& selection ) {
  // we are going to do a dangerous cast below to avoid a copy.  Ensure that it
  // will work.
  // FIXME: this can be a compile time assertion.  If we go this route,
  // implement it.
  assert( sizeof( xdm::CoordinateArray<>::size_type ) == sizeof( hsize_t ) );

  xdm::CoordinateArray<> coords = selection.coordinates();
  
  // HDF requires a numberOfElements x rank array for selection. Set up an array
  // of pointers in the static buffer to simulate this from the contiguous
  // input.
  xdm::CoordinateArray<>::size_type numberOfElements = coords.numberOfElements();
  xdm::CoordinateArray<>::size_type rank = coords.rank();
  sPointerBuffer.resize( numberOfElements );
  int count = 0;
  for ( xdm::CoordinateArray<>::size_type index = 0; 
    index < numberOfElements; 
    index += rank ) {
    sPointerBuffer[count++] = reinterpret_cast< const hsize_t* >( &coords.values()[index] );
  }

  H5Sselect_elements( 
    mIdent, 
    H5S_SELECT_SET, 
    coords.numberOfElements(), 
    sPointerBuffer[0] );
}
*/

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

