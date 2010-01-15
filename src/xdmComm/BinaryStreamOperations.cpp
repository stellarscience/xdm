#include <xdmComm/BinaryStreamOperations.hpp>

XDM_COMM_NAMESPACE_BEGIN

BinaryIStream& operator>>( BinaryIStream& istr, xdm::DataShape<>& v ) {
  // read the rank followed by the sizes.
  xdm::DataShape<>::size_type rank;
  istr >> rank;
  v.setRank( rank );
  for ( int i = 0; i < rank; i++ ) {
    istr >> v[i];
  }
  return istr;
}

BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::DataShape<>& v ) {
  // Write the rank followed by the sizes.
  xdm::DataShape<>::size_type rank = v.rank();
  ostr << rank;
  for ( int i = 0; i < rank; i++ ) {
    ostr << v[i];
  }
  return ostr;
}

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

