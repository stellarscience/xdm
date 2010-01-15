#include <gtest/gtest.h>

#include <xdm/DataSelectionVisitor.hpp>

#include <xdm/DataSelection.hpp>
#include <xdm/AllDataSelection.hpp>
#include <xdm/HyperslabDataSelection.hpp>

using xdm::DataSelectionVisitor;
using xdm::DataSelection;
using xdm::AllDataSelection;
using xdm::HyperslabDataSelection;

// To add a visitation test for a new DataSelection class, add a virtual method
// that fails for your type to CheckVisitationBase below and add your type to
// the ::testing::Types typedef below.

// Visitor that fails on everything
struct CheckVisitationBase : DataSelectionVisitor {
  virtual void apply( DataSelection& ) { FAIL(); }
  virtual void apply( AllDataSelection& ) { FAIL(); }
  virtual void apply( HyperslabDataSelection& ) { FAIL(); }
};

// succeeds with T input, fails otherwise
template< typename T >
struct Succeed : CheckVisitationBase {
  void apply( T& ) { SUCCEED(); }
};

template< typename T >
class CheckVisitation : public ::testing::Test {
public:
  T* t;
  Succeed< T > visitor;
  CheckVisitation() : t( new T ) {}
  ~CheckVisitation() { delete t; }
};

typedef ::testing::Types<
  DataSelection,
  AllDataSelection,
  HyperslabDataSelection > SelectionTypes;
TYPED_TEST_CASE( CheckVisitation, SelectionTypes );

TYPED_TEST( CheckVisitation, success ) {
  // remove specific type information for a real check
  DataSelection* baseobj = this->t;
  baseobj->accept( this->visitor );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

