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
#ifndef xdmFormat_IoExcept_hpp
#define xdmFormat_IoExcept_hpp

#include <sstream>
#include <stdexcept>

#include <xdmFormat/NamespaceMacro.hpp>

XDM_FORMAT_NAMESPACE_BEGIN

/// Exception to be thrown when an error occurs while reading an input file.
class ReadError : public std::runtime_error {
public:
  ReadError( const std::string& message ) :
    std::runtime_error( message.c_str() ) {}
  virtual ~ReadError() throw() {}
};

/// Base exception for errors that occur while writing data to disk.
class WriteError : public std::runtime_error {
public:
  WriteError( const std::string& message ) :
    std::runtime_error( message.c_str() ) {}
  virtual ~WriteError() throw() {}
};

/// Exception to be thrown when a read error with knowledge of file structure,
/// including a line and column number is encountered.
class FileReadError : public ReadError {
  std::string mFile;
  int mLine;
  int mColumn;

public:
  FileReadError(
    const std::string& file,
    int line,
    int column,
    const std::string& reason ) :
    ReadError( reason ),
    mFile( file ),
    mLine( line ),
    mColumn( column ) {}
  virtual ~FileReadError() throw() {}
  virtual const char* what() const throw() {
    try {
      static std::string message;
      if ( message.empty() ) {
        std::stringstream ss;
        ss << "Error reading " << mFile << " at line ";
        ss << mLine << ", column " << mColumn;
        ss << ": " << ReadError::what();
        message = ss.str();
      }
      return message.c_str();
    } catch ( ... ) {
      return ReadError::what();
    }
  }

  int line() const { return mLine; }
  int column() const { return mColumn; }
};

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_IoExcept_hpp
