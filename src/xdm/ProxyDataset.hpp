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
#ifndef xdm_ProxyDataset_hpp
#define xdm_ProxyDataset_hpp

#include <xdm/Dataset.hpp>
#include <xdm/RefPtr.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Dataset that has a Dataset and is a Dataset. Useful for Dataset subclasses
/// that need to redefine subsets of generic Dataset behavior while remaining
/// independent of the specific Dataset implementation. The default
/// implementations of all Dataset methods are pass throughs to the underlying
/// Dataset.
class ProxyDataset : public Dataset {
public:
  ProxyDataset( RefPtr< Dataset > dataset );
  virtual ~ProxyDataset();

  /// Set the inner Dataset.
  void setInnerDataset( RefPtr< Dataset > dataset );
  /// Get the innder Dataset.
  RefPtr< Dataset > innerDataset();

  /// Pass through implementation to update the proxy and the inner Dataset.
  virtual void update();

  /// Pass through implementation to get the format of the inner Dataset.
  virtual const char* format();
  /// Pass through implementation to get the inner Dataset to write it's text
  /// content.
  virtual void writeTextContent( XmlTextContent& text );

protected:
  /// Pass through implementation to initialize the inner Dataset.
  virtual DataShape<> initializeImplementation(
    xdm::primitiveType::Value type,
    const xdm::DataShape<>& shape,
    const xdm::Dataset::InitializeMode& mode );

  /// Pass through implementation to serialize data to the inner Dataset.
  virtual void serializeImplementation(
    const xdm::StructuredArray* data,
    const xdm::DataSelectionMap& selectionMap );

  /// Pass through implementation to deserialize data from the inner Dataset.
  virtual void deserializeImplementation(
    xdm::StructuredArray* data,
    const xdm::DataSelectionMap& selectionMap );

  /// Pass through implementation to finalize the inner Dataset.
  virtual void finalizeImplementation();

private:
  RefPtr< Dataset > mDataset;
};

XDM_NAMESPACE_END

#endif // xdm_ProxyDataset_hpp
