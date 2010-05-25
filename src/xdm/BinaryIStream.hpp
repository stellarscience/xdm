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
#ifndef xdm_BinaryIStream_hpp
#define xdm_BinaryIStream_hpp

#include <xdm/BinaryIosBase.hpp>



namespace xdm {

/// Stream class for reading binary object representations from a
/// BinaryStreamBuffer.
class BinaryIStream : virtual public BinaryIosBase {
public:
  
  /// Constructor takes a pointer to a stream buffer.
  BinaryIStream( BinaryStreamBuffer* buf ); 
  virtual ~BinaryIStream();

  /// Read and return the next character in the stream.
  int get();
  /// Read a single character from the stream into the parameter.
  /// @param c Location to write the next character in the stream.
  BinaryIStream& get( char& c );
  /// Read a block of n characters from the stream.
  BinaryIStream& read( char* const p, size_t n );

  //-- Exraction Operators --//

  BinaryIStream& operator>>( short& n );
  BinaryIStream& operator>>( int& n );
  BinaryIStream& operator>>( long& n );

  BinaryIStream& operator>>( unsigned short& n );
  BinaryIStream& operator>>( unsigned int& n );
  BinaryIStream& operator>>( unsigned long& n );

  BinaryIStream& operator>>( float& n );
  BinaryIStream& operator>>( double& n );
  BinaryIStream& operator>>( long double& n );
  
  BinaryIStream& operator>>( bool& n );

  BinaryIStream& sync();
};

BinaryIStream& operator>>( BinaryIStream& ostr, char& c );

} // namespace xdm

#endif // xdm_BinaryIStream_hpp

