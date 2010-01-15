#include <xdm/DataShape.hpp>
#include <xdm/ThrowMacro.hpp>

#include <algorithm>
#include <istream>
#include <iterator>
#include <sstream>
#include <stdexcept>

XDM_NAMESPACE_BEGIN

DataShape<> makeShape( const std::string& dimensions ) {
  typedef DataShape<>::size_type SizeType;
  std::stringstream s( dimensions );
  std::vector< SizeType > result;
  std::copy( 
    std::istream_iterator< SizeType >( s ), 
    std::istream_iterator< SizeType >(),
    std::back_inserter( result ) );
  if ( !s.eof() ) {
    // we didn't reach the end of the string, format error
    XDM_THROW( std::invalid_argument( 
      std::string("Invalid dimensions ") + dimensions ) );
  }
  
  /// copy the dimensions into a new shape.
  DataShape<> ret( result.size() );
  std::copy( result.begin(), result.end(), ret.begin() );
  return ret;
}

DataShape<> makeShape( DataShape<>::size_type i ) {
  DataShape<> result( 1 );
  result[0] = i;
  return result;
}

DataShape<> makeShape( DataShape<>::size_type i, DataShape<>::size_type j ){
  DataShape<> result( 2 );
  result[0] = i;
  result[1] = j;
  return result;
}

DataShape<> makeShape( 
  DataShape<>::size_type i,
  DataShape<>::size_type j,
  DataShape<>::size_type k ) {
  DataShape<> result( 3 );
  result[0] = i;
  result[1] = j;
  result[2] = k;
  return result;
}

DataShape<> makeShape( 
  DataShape<>::size_type i,
  DataShape<>::size_type j,
  DataShape<>::size_type k,
  DataShape<>::size_type l ) {
  DataShape<> result( 4 );
  result[0] = i;
  result[1] = j;
  result[2] = k;
  result[3] = l;
  return result;
}

DataShape<> makeContraction( const DataShape<>& space, DataShape<>::size_type n ) {
  DataShape<> result( space.rank() );
  for ( int i = 0; i < space.rank(); i++ ) {
    result[i] = std::max( space[i] - n, 1lu );
  }
  return result;
}

XDM_NAMESPACE_END

