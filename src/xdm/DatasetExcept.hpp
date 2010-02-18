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
#ifndef xdm_DatasetExcept_hpp
#define xdm_DatasetExcept_hpp

#include <stdexcept>
#include <string>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Base class for dataset related exceptions.
class DatasetError : public std::runtime_error {
  std::string mName;
public:
  DatasetError( const std::string& name, const std::string& message  ) :
    std::runtime_error( name + ": " + message ),
    mName( name ) {}
  ~DatasetError() throw() {}
  const std::string& name() const { return mName; }
};

/// Exception to be thrown when a dataset opened for reading does not exist.
class DatasetNotFound : public DatasetError {
public:
  DatasetNotFound( const std::string& name ) :
    DatasetError( name, "Dataset not found" ) {}
};

/// Exception to be thrown when a dataset opened for writing already exists.
class DatasetExists : public DatasetError {
public:
  DatasetExists( const std::string& name ) :
    DatasetError( name, "Dataset already exists" ) {}
};

/// Exception to be thrown when the dataspace of a dataset opened for reading or
/// modification is different than the dataspace of the dataset specified on
/// disk.
class DataspaceMismatch : public DatasetError {
public:
  DataspaceMismatch( 
    const std::string& name, 
    xdm::DataShape<> shape1,
    xdm::DataShape<> shape2 ) :
    DatasetError( name, "Requested space does not match space on disk" ),
    mShape1( shape1 ),
    mShape2( shape2 ),
    mWhat() {}
  
  virtual ~DataspaceMismatch() throw() {}

  virtual const char* what() const throw() {
    try {
      if ( mWhat.empty() ) {
        std::stringstream ss;
        ss << DatasetError::what() << ": ";
        ss << mShape1 << " does not match " << mShape2;
        mWhat = ss.str();
      }
      return mWhat.c_str();
    } catch ( ... ) {
      // do nothing
    }
    return "Dataspace mismatch";
  }
private:
  xdm::DataShape<> mShape1;
  xdm::DataShape<> mShape2;
  mutable std::string mWhat;
};

/// Exception to be thrown when the type of a dataset opened for reading or
/// modification is different from the type of the dataset specified on disk.
class DatatypeMismatch : public DatasetError {
public:
  DatatypeMismatch( const std::string& name ) :
    DatasetError( name, "Requested type does not match type on disk" ) {}
};

/// Exception to be thrown when the number of selected elements in an array and
/// the number of selected elements in a dataset differ during serialization or
/// deserialization.
class DataSizeMismatch : public DatasetError {
private:
  size_t mDatasetSize;
  size_t mArraySize;
public:
  DataSizeMismatch( const std::string& name, size_t datasetSize, size_t arraySize ):
    DatasetError( name, "" ) {}
  virtual const char* what() const throw() {

    // Code Review Matter (open): Exception what()
    // Did you consider constructing this exception message only when
    // a client calls the what() function? This would require an exception
    // subclass and would provide a way to access the data members used
    // to make the message without having to resort to string parsing.
    // -- K. R. Walker on 2010-01-19
    try {
      std::stringstream ss;
      ss << "Data size mismatch: array has " << mArraySize;
      ss << " elements, dataset has " << mDatasetSize << " elements";
      return ss.str().c_str();
    } catch ( ... ) {
      // do nothing
    }
    
    // If an error occurred while building the string, return a simple c-string.
    return "Data size mismatch";
  }
  size_t datasetSize() const { return mDatasetSize; }
  size_t arraySize() const { return mArraySize; }
};

XDM_NAMESPACE_END

#endif // xdm_DatasetExcept_hpp
