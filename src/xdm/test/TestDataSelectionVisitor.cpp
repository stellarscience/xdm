//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE DataSelectionVisitor 
#include <boost/test/unit_test.hpp>

#include <xdm/DataSelectionVisitor.hpp>

#include <xdm/DataSelection.hpp>
#include <xdm/AllDataSelection.hpp>
#include <xdm/HyperslabDataSelection.hpp>

namespace {

using xdm::DataSelectionVisitor;
using xdm::DataSelection;
using xdm::AllDataSelection;
using xdm::HyperslabDataSelection;

// To add a visitation test for a new DataSelection class, add a virtual method
// that fails for your type to CheckVisitationBase below and add a test case
// that instantiates the checkVisitation function template with your type.

char const * const kErrorMessage = "Data selection visit failed";

// Visitor that fails on everything
struct CheckVisitationBase : DataSelectionVisitor {
  virtual void apply( const DataSelection& ) { BOOST_ERROR( kErrorMessage ); }
  virtual void apply( const AllDataSelection& ) { BOOST_ERROR( kErrorMessage ); }
  virtual void apply( const HyperslabDataSelection& ) { BOOST_ERROR( kErrorMessage ); }
};

// succeeds with T input, fails otherwise
template< typename T >
struct Succeed : CheckVisitationBase {
  void apply( const T& ) {}
};

template< typename T >
class CheckVisitation {
public:
  T* t;
  Succeed< T > visitor;
  CheckVisitation() : t( new T ) {}
  ~CheckVisitation() { delete t; }
};

template< typename T > void checkVisitation() {
  CheckVisitation< T > test;
  test.t->accept( test.visitor );
}

BOOST_AUTO_TEST_CASE( checkVisitationDataSelection ) {
  checkVisitation< DataSelection >();
}

BOOST_AUTO_TEST_CASE( checkVisitationAllDataSelection ) {
  checkVisitation< AllDataSelection >();
}

BOOST_AUTO_TEST_CASE( checkVisitationHyperslabDataSelection ) {
  checkVisitation< HyperslabDataSelection >();
}

} // namespace

