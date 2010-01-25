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
#ifndef xdm_XmlExcept_hpp
#define xdm_XmlExcept_hpp

#include <sstream>
#include <stdexcept>
#include <string>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Base class for Xml related exceptions.
class XmlError : public std::runtime_error {
  std::string mTag;
public:
  XmlError( const std::string& tag ) :
    std::runtime_error( "Xml Error" ),
    mTag( tag ) {}
  ~XmlError() throw() {}
  virtual const char* what() const throw() {
    try {
      std::stringstream msg;
      msg << mTag << ": " << std::runtime_error::what();
      return msg.str().c_str();
    } catch ( ... ) {
      // do nothing
    }
    return "Unknown XML error";
  }
  const std::string& tag() const { return mTag; }
};

/// Base class for errors involving XML attributes.
class AttributeError : public XmlError {
  std::string mAttribute;
  std::string mReason;
public:
  AttributeError(
    const std::string& tag,
    const std::string& attribute,
    const std::string& reason ) :
    XmlError( tag ),
    mAttribute( attribute ),
    mReason( reason ) {}
  ~AttributeError() throw() {}
  virtual const char * what() const throw() {
    try {
      std::stringstream msg;
      msg << tag() << ": attribute " << attribute() << " " << mReason;
      return msg.str().c_str();
    } catch ( ... ) {
      // do nothing
    }
    return "Unknown attribute error";
  }
  const std::string& attribute() const { return mAttribute; }
};

/// Exception to be thrown when an attribute does not exist.
class AttributeDoesNotExist : public AttributeError {
public:
  AttributeDoesNotExist( const std::string& tag, const std::string& attribute ) :
    AttributeError( tag, attribute, "does not exist" ) {}
};

/// Exception to be thrown when an attribute has an incorrect type.
class AttributeTypeError : public AttributeError {
public:
  AttributeTypeError( const std::string& tag, const std::string& attribute ) :
    AttributeError( tag, attribute, "has incorrect type" ) {}
};

XDM_NAMESPACE_END

#endif // xdm_XmlExcept_hpp
