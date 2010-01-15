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

XDM_NAMESPACE_END

