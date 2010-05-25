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
#ifndef xdm_BinaryIosBase_hpp
#define xdm_BinaryIosBase_hpp

#include <xdm/BinaryStreamBuffer.hpp>



namespace xdm {

/// Base class that manages access to an underlying stream buffer for streaming
/// input and output of binary serialized objects.
template< typename StreamBufT >
class BasicBinaryIosBase {
private:
  StreamBufT* mStreamBuffer;

  // non-copyable
  BasicBinaryIosBase( const BasicBinaryIosBase& );
  BasicBinaryIosBase& operator=( const BasicBinaryIosBase& );

public:

  typedef StreamBufT stream_buffer;

  /// The constructor takes a reference to the underlying stream buffer to use.
  /// The buffer's position pointer is reset to the beginning of the buffer upon
  /// construction.
  BasicBinaryIosBase( StreamBufT* buffer ) :
    mStreamBuffer( buffer ) {
    buffer->pubseekpos( 0 );
  }

  ~BasicBinaryIosBase() {}

  /// Get the mutable stream buffer.
  stream_buffer* rdbuf() { return mStreamBuffer; } 
};

typedef BasicBinaryIosBase< BinaryStreamBuffer > BinaryIosBase;

} // namespace xdm

#endif // xdm_BinaryIosBase_hpp

