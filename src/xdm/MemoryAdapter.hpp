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
#ifndef xdm_MemoryAdapter_hpp
#define xdm_MemoryAdapter_hpp

#include <xdm/ReferencedObject.hpp>
#include <xdm/StructuredArray.hpp>



namespace xdm {

class Dataset;

/// Base class for use when mapping application data to array accessible data.
/// Client applications can implement this class to provide ordered data access
/// to data layouts in memory with different organizational requirements. In
/// addition to data access, this class defines the interface for reading and
/// writing custom application memory layouts to and from Dataset objects.
///
/// The ArrayAdapter subclass provides an implementation for simple contiguous
/// array based access.
/// @see ArrayAdapter
class MemoryAdapter : public xdm::ReferencedObject {
public:
  MemoryAdapter( bool isDynamic = false );
  virtual ~MemoryAdapter();

  /// Determine if the data changes as time evolves.  Guarantees that data will
  /// be written every time a write is requested.
  bool isDynamic() const;
  /// Set whether or not the data changes as time evolves.
  void setIsDynamic( bool isDynamic );

  /// Determine if the data needs to be updated to reflect external changes.
  bool needsUpdate() const;
  /// Specify that the data needs to be updated to reflect external changes.
  void setNeedsUpdate( bool needsUpdate );

  /// Determine if the data currently requires writing.
  bool requiresWrite() const;

  /// Determine if the data is memory resident and therefore never needs to be
  /// read from disk.
  bool isMemoryResident() const;
  /// Specify that the data is memory resident and therefore never needs to be
  /// read from disk.
  void setIsMemoryResident( bool isMemoryResident );

  /// Write the data to the specified dataset.
  void write( Dataset* dataset );

  /// Read data in from the specified dataset.
  void read( Dataset* dataset );

  /// Get a const array representation of the underlying data. Inheritors
  /// should implement this method to provide a StructuredArray subclass that
  /// provides random access to their data.
  virtual RefPtr< const StructuredArray > array() const = 0;

  /// Get a const array representation of the underlying data. The default
  /// implementation calls the const version.
  virtual RefPtr< StructuredArray > array();

protected:
  /// Method to be implemented by inheritors to define the data to be written to
  /// the dataset.
  virtual void writeImplementation( Dataset* dataset ) = 0;
  /// Method to be implemented by inheritors to define how to read data in from
  /// the specified dataset.
  virtual void readImplementation( Dataset* dataset ) = 0;

private:
  bool mIsDynamic;
  bool mNeedsUpdate;
  bool mIsMemoryResident;
};

} // namespace xdm

#endif // xdm_MemoryAdapter_hpp

