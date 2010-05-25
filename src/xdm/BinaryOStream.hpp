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
#ifndef xdm_BinaryOStream_hpp
#define xdm_BinaryOStream_hpp

#include <xdm/BinaryIosBase.hpp>



namespace xdm {

/// Stream class for writing object binary representations to a
/// BinaryStreamBuffer.
class BinaryOStream : virtual public BinaryIosBase {
public:
  
  /// Constructor takes a pointer to a stream buffer.
  BinaryOStream( BinaryStreamBuffer* buf ); 
  virtual ~BinaryOStream();

  /// Put a single character to the stream.
  BinaryOStream& put( char c );
  /// Put n characters to the stream.
  BinaryOStream& write( const char* p, size_t n );

  //-- Insertion Operators --//

  BinaryOStream& operator<<( short n );
  BinaryOStream& operator<<( int n );
  BinaryOStream& operator<<( long n );

  BinaryOStream& operator<<( unsigned short n );
  BinaryOStream& operator<<( unsigned int n );
  BinaryOStream& operator<<( unsigned long n );

  BinaryOStream& operator<<( float n );
  BinaryOStream& operator<<( double n );
  BinaryOStream& operator<<( long double n );
  
  BinaryOStream& operator<<( bool n );

  BinaryOStream& flush();

  /// Insertion operator that takes a function pointer and invokes it on the
  /// stream.  This allows the stream to support manipulators.
  BinaryOStream& operator<<( BinaryOStream& (*f)( BinaryOStream& ) ) {
    return f( *this );
  }
};

BinaryOStream& operator<<( BinaryOStream& ostr, char c );
BinaryOStream& operator<<( BinaryOStream& ostr, unsigned char c );

/// Stream manipulator that flushes the stream.
inline BinaryOStream& flush( BinaryOStream& ostr ) { return ostr.flush(); }

} // namespace xdm

#endif // xdm_BinaryOStream_hpp

