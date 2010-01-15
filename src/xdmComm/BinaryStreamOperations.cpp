#include <xdmComm/BinaryStreamOperations.hpp>

#include <xdm/DataSelectionVisitor.hpp>
#include <xdm/ThrowMacro.hpp>

#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>

XDM_COMM_NAMESPACE_BEGIN

namespace {

// Functor to write a type out to a BinaryOStream.
template< typename T >
struct OutputObject {
  BinaryOStream& mOStr;
  OutputObject( BinaryOStream& ostr ) : mOStr( ostr ) {}
  void operator()( const T& object ) {
    mOStr << object;
  }
};

// Functor to read a type int from a BinaryIStream.
template< typename T >
struct InputObject {
  BinaryIStream& mIStr;
  InputObject( BinaryIStream& istr ) : mIStr( istr ) {}
  void operator()( T& object ) {
    mIStr >> object;
  }
};

// Visitor implementation that serializes a DataSelection subclass depending on
// its type.
class DataSelectionOutputVisitor : public xdm::DataSelectionVisitor {
private:
  BinaryOStream& mOStr;

public:

  enum SupportedSelection {
    kAllDataSelection = 0,
    kHyperslabDataSelection
  };

  DataSelectionOutputVisitor( BinaryOStream& ostr ) : mOStr( ostr ) {}
  virtual ~DataSelectionOutputVisitor() {}

  virtual void apply( const xdm::AllDataSelection& selection ) {
    mOStr << kAllDataSelection;
    mOStr << selection;
  }

  virtual void apply( const xdm::HyperslabDataSelection& selection ) {
    mOStr << kHyperslabDataSelection;
    mOStr << selection;
  }
};

// Proxy DataSelection that holds a DataSelection and forwards visitation calls
// to the enclosed object. This allows it to look like a specific sublcass of
// DataSelection to outside clients.
class DataSelectionInputProxy : public xdm::DataSelection {
private:
  xdm::RefPtr< xdm::DataSelection > mRealSelection;

public:
  
  virtual void accept( xdm::DataSelectionVisitor& v ) const {
    mRealSelection->accept( v );
  }

  void setRealSelection( xdm::DataSelection* s ) {
    mRealSelection = s;
  }
};

// extraction operator that configures a DataSelectionInputProxy so it will
// contain the right subclass of DataSelection for the input key written out by
// a DataSelectionOutputVisitor.
BinaryIStream& operator>>( BinaryIStream& istr, DataSelectionInputProxy& v ) {
  int keyRead;
  istr >> keyRead;
  DataSelectionOutputVisitor::SupportedSelection selectionKey = 
    static_cast< DataSelectionOutputVisitor::SupportedSelection >( keyRead );

  switch ( selectionKey ) {
  case DataSelectionOutputVisitor::kAllDataSelection: {
    std::auto_ptr< xdm::AllDataSelection > selection( 
      new xdm::AllDataSelection );
    v.setRealSelection( selection.release() );
    break;
  }
  case DataSelectionOutputVisitor::kHyperslabDataSelection: {
    std::auto_ptr< xdm::HyperslabDataSelection > selection(
      new xdm::HyperslabDataSelection );
    istr >> *selection;
    v.setRealSelection( selection.release() );
    break;
  }
  default:
    XDM_THROW( std::runtime_error( "Unknown selection key" ) );
    break;
  }
  return istr;
}

} // namespace anon

// NOTE: in this file, pairs of corresponding IO operators for a given type are
// separated  with the //---... blocks below.  Updating one will generally
// require updating the other.

//-----------------------------------------------------------------------------
BinaryIStream& operator>>( BinaryIStream& istr, xdm::AllDataSelection& v ) {
  return istr;
}

BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::AllDataSelection& v ) {
  return ostr;
}

//-----------------------------------------------------------------------------
BinaryIStream& operator>>( BinaryIStream& istr, xdm::DataSelectionMap& v ) {
  xdm::RefPtr< DataSelectionInputProxy > domain( new DataSelectionInputProxy );
  xdm::RefPtr< DataSelectionInputProxy > range( new DataSelectionInputProxy );
  istr >> *domain;
  istr >> *range;
  v.setDomain( domain );
  v.setRange( range );
  return istr;
}

BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::DataSelectionMap& v ) {
  DataSelectionOutputVisitor ov( ostr );
  v.domain()->accept( ov );
  v.range()->accept( ov );
  return ostr;
}

//-----------------------------------------------------------------------------
BinaryIStream& operator>>( BinaryIStream& istr, xdm::DataShape<>& v ) {
  // read the rank followed by the sizes.
  xdm::DataShape<>::size_type rank;
  istr >> rank;
  v.setRank( rank );
  std::for_each( v.begin(), v.end(), 
    InputObject< xdm::DataShape<>::size_type >( istr ) );
  return istr;
}

BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::DataShape<>& v ) {
  // Write the rank followed by the sizes.
  xdm::DataShape<>::size_type rank = v.rank();
  ostr << rank;
  std::for_each( v.begin(), v.end(), 
    OutputObject< xdm::DataShape<>::size_type >( ostr ) );
  return ostr;
}

//-----------------------------------------------------------------------------
BinaryIStream& operator>>( BinaryIStream& istr, xdm::HyperSlab<>& v ) {
  xdm::DataShape<> shape;
  istr >> shape;
  v.setShape( shape );
  std::for_each( v.beginStart(), v.endStart(), 
    InputObject< xdm::HyperSlab<>::size_type >( istr ) );
  std::for_each( v.beginStride(), v.endStride(), 
    InputObject< xdm::HyperSlab<>::size_type >( istr ) );
  std::for_each( v.beginCount(), v.endCount(), 
    InputObject< xdm::HyperSlab<>::size_type >( istr ) );
  return istr;
}

BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::HyperSlab<>& v ) {
  ostr << v.shape();
  std::for_each( v.beginStart(), v.endStart(), 
    OutputObject< xdm::HyperSlab<>::size_type >( ostr ) );
  std::for_each( v.beginStride(), v.endStride(), 
    OutputObject< xdm::HyperSlab<>::size_type >( ostr ) );
  std::for_each( v.beginCount(), v.endCount(), 
    OutputObject< xdm::HyperSlab<>::size_type >( ostr ) );
  return ostr;
}

//-----------------------------------------------------------------------------
BinaryIStream& operator>>( BinaryIStream& istr, xdm::HyperslabDataSelection& v ) {
  xdm::HyperSlab<> slab;
  istr >> slab;
  v.setHyperslab( slab );
  return istr;
}

BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::HyperslabDataSelection& v ) {
  ostr << v.hyperslab();
  return ostr;
}

//-----------------------------------------------------------------------------
BinaryIStream& operator>>( BinaryIStream& istr, xdm::primitiveType::Value& v ) {
  int value;
  istr >> value;
  v = static_cast< xdm::primitiveType::Value >( value );
  return istr;
}

BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::primitiveType::Value& v ) {
  ostr << static_cast< int >( v );
  return ostr;
}

//-----------------------------------------------------------------------------
BinaryIStream& operator>>( BinaryIStream& istr, xdm::StructuredArray& v ) {
  // type - shape - data
  xdm::primitiveType::Value type;
  xdm::DataShape<> shape;
  istr >> type >> shape;
  v.setDataType( type );
  v.setShape( shape );
  istr.read( reinterpret_cast< char* >( v.data() ), v.memorySize() );
  return istr;
}

BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::StructuredArray& v ) {
  // type - shape - data...
  ostr << v.dataType() << v.shape();
  ostr.write( reinterpret_cast< const char * >( v.data() ), v.memorySize() );
  return ostr;
}


XDM_COMM_NAMESPACE_END

