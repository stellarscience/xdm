//=============================================================================
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
#include <xdmf/XmfWriter.hpp>

#include <xdmf/TemporalCollection.hpp>
#include <xdmf/VirtualDataset.hpp>

#include <xdm/CollectMetadataOperation.hpp>
#include <xdm/SerializeDataOperation.hpp>
#include <xdm/XmlObject.hpp>
#include <xdm/XmlOutputStream.hpp>

#include <fstream>

#include <stdexcept>

XDMF_NAMESPACE_BEGIN

namespace {

void writeItemImpl(
  xdm::RefPtr< xdm::Item > item,
  const xdm::FileSystemPath& path ) {
  xdm::RefPtr< xdm::XmlObject > xdmfRoot( new xdm::XmlObject( "Xdmf" ) );
  xdmfRoot->appendAttribute( "Version", "2.1" );

  xdm::RefPtr< xdm::XmlObject > domain( new xdm::XmlObject( "Domain" ) );
  xdmfRoot->appendChild( domain );

  xdm::CollectMetadataOperation metadata;
  item->accept( metadata );
  domain->appendChild( metadata.result() );

  std::ofstream file( path.pathString().c_str() );
  file << "<?xml version='1.0'?>" << std::endl;
  xdm::XmlOutputStream xmlStream( file );
  xmlStream.writeObject( xdmfRoot );
  xmlStream.closeStream();

  xdm::SerializeDataOperation serialize( xdm::Dataset::kCreate );
  item->accept( serialize );
}

} // namespace

XmfWriter::XmfWriter() :
  xdmFormat::Writer(),
  mWriters() {
}

XmfWriter::~XmfWriter() {
  for ( FileMap::iterator file = mWriters.begin(); file != mWriters.end(); ++file ) {
    file->second->close();
  }
}

void XmfWriter::writeItem(
  xdm::RefPtr< xdm::Item > item,
  const xdm::FileSystemPath& path ) {
  writeItemImpl( item, path );
}

bool XmfWriter::update(
  xdm::RefPtr<xdm::Item> item,
  const xdm::FileSystemPath &path,
  std::size_t timeStep ) {
  writeItemImpl( item, path );
  return true;
}

XDMF_NAMESPACE_END
