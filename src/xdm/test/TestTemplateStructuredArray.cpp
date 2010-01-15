
#include <xdm/DataShape.hpp>
#include <xdm/TemplateStructuredArray.hpp>
#include <xdm/RefPtr.hpp>

#include <iostream>

template< typename T > 
bool test() {
  xdm::DataShape<> shape(1);
  shape[0] = 1;
  T value = T();
  xdm::RefPtr< xdm::TemplateStructuredArray< T > > array( 
    new xdm::TemplateStructuredArray< T >( &value, shape ) );
  T* result = array->begin();
  return !( result == &value );
}

int main( int argc, char* argv[] ) {
  bool fail = false;
  fail = test<char>();
  fail = test<short>();
  fail = test<int>();
  fail = test<long int>();
  fail = test<unsigned char>();
  fail = test<unsigned short>();
  fail = test<unsigned int>();
  fail = test<long unsigned int>();
  fail = test<float>();
  fail = test<double>();
  return fail;
}

