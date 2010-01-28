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
#include <xdm/ProxyDataset.hpp>

XDM_NAMESPACE_BEGIN

ProxyDataset::ProxyDataset( RefPtr< Dataset > dataset ) : mDataset( dataset ) {
}

ProxyDataset::~ProxyDataset() {
}

void ProxyDataset::setInnerDataset( RefPtr< Dataset > dataset ) {
  mDataset = dataset;
}

RefPtr< Dataset > ProxyDataset::innerDataset() {
  return mDataset;
}

void ProxyDataset::update() {
  Dataset::update();
  mDataset->update();
}

const char* ProxyDataset::format() {
  return mDataset->format();
}

void ProxyDataset::writeTextContent( XmlTextContent& text ) {
  mDataset->writeTextContent( text );
}

void ProxyDataset::initializeImplementation(
  primitiveType::Value type,
  const DataShape<>& shape,
  const InitializeMode& mode ) {
  mDataset->initialize( type, shape, mode );
}

void ProxyDataset::serializeImplementation(
  const StructuredArray* data,
  const DataSelectionMap& selectionMap ) {
  mDataset->serialize( data, selectionMap );
}

void ProxyDataset::deserializeImplementation(
  StructuredArray* data,
  const DataSelectionMap& selectionMap ) {
  mDataset->deserialize( data, selectionMap );
}

void ProxyDataset::finalizeImplementation() {
  mDataset->finalize();
}

XDM_NAMESPACE_END
