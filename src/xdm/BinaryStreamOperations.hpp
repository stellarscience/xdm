//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.      
//                                                                             
// This file is part of XDM                                                    
//                                                                             
// This program is free software: you can redistribute it and/or modify it     
// under the terms of the GNU Lesser General Public License as published by    
// the Free Software Foundation, either version 3 of the License, or (at your  
// option) any later version.                                                  
//                                                                             
// This program is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        
// License for more details.                                                   
//                                                                             
// You should have received a copy of the GNU Lesser General Public License    
// along with this program.  If not, see <http://www.gnu.org/licenses/>.       
//                                                                             
//------------------------------------------------------------------------------
#ifndef xdm_BinaryStreamOperations_hpp
#define xdm_BinaryStreamOperations_hpp

#include <xdm/AllDataSelection.hpp>
#include <xdm/BinaryIStream.hpp>
#include <xdm/BinaryOStream.hpp>
#include <xdm/ByteArray.hpp>
#include <xdm/DataSelection.hpp>
#include <xdm/DataSelectionMap.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/PrimitiveType.hpp>
#include <xdm/StructuredArray.hpp>
#include <xdm/XmlObject.hpp>

#include <stdexcept>
#include <string>
#include <utility>

#include <xdm/NamespaceMacro.hpp>
#include <xdm/ThrowMacro.hpp>

XDM_NAMESPACE_BEGIN

class NullPointerException : public std::runtime_error {
  NullPointerException() : std::runtime_error( "Null pointer exception" ) {}
};

BinaryIStream& operator>>( BinaryIStream& istr, std::string& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const std::string& v );

template< typename T, typename U >
BinaryIStream& operator>>( BinaryIStream& istr, std::pair< T, U >& v ) {
  istr >> v.first;
  istr >> v.second;
  return istr;
}

template< typename T, typename U >
BinaryOStream& operator<<( BinaryOStream& ostr, const std::pair< T, U >& v ) {
  ostr << v.first;
  ostr << v.second;
  return ostr;
}

BinaryIStream& operator>>( BinaryIStream& istr, xdm::AllDataSelection& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::AllDataSelection& v );

BinaryIStream& operator>>( BinaryIStream& istr, xdm::DataSelectionMap& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::DataSelectionMap& v );

BinaryIStream& operator>>( BinaryIStream& istr, xdm::DataShape<>& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::DataShape<>& v );

BinaryIStream& operator>>( BinaryIStream& istr, xdm::HyperSlab<>& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::HyperSlab<>& v );

BinaryIStream& operator>>( BinaryIStream& istr, xdm::HyperslabDataSelection& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::HyperslabDataSelection& v );

BinaryIStream& operator>>( BinaryIStream& istr, xdm::primitiveType::Value& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::primitiveType::Value& v );

/// This inserter assumes that the output StructuredArray already has at least
/// enough space allocated to hold the data.
BinaryIStream& operator>>( BinaryIStream& istr, ByteArray& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::StructuredArray& v );

BinaryIStream& operator>>( BinaryIStream& istr, xdm::XmlObject& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::XmlObject& v );

/// Convenience functor to write a type out to a BinaryOStream.
template< typename T >
struct OutputObject {
  BinaryOStream& mOStr;
  OutputObject( BinaryOStream& ostr ) : mOStr( ostr ) {}
  void operator()( const T& object ) {
    mOStr << object;
  }
};

/// Convenience functor to write a RefPtr to a BinaryOStream.
template< typename T >
struct OutputObject< RefPtr< T > > {
  BinaryOStream& mOStr;
  OutputObject( BinaryOStream& ostr ) : mOStr( ostr ) {}
  void operator()( const RefPtr< T >& object ) {
    mOStr << *object;
  }
};

/// Convenience functor to read a type in from a BinaryIStream.
template< typename T >
struct InputObject {
  BinaryIStream& mIStr;
  InputObject( BinaryIStream& istr ) : mIStr( istr ) {}
  void operator()( T& object ) {
    mIStr >> object;
  }
};

XDM_NAMESPACE_END

#endif // xdm_BinaryStreamOperations_hpp

