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

#include <xdmHdf/AttachHdfDatasetOperation.hpp>

#include <fstream>

#include <stdexcept>

XDMF_NAMESPACE_BEGIN

XmfWriter::XmfWriter() :
  xdmFormat::Writer(),
  mSeries(),
  mIsOpen( false ) {
}

XmfWriter::~XmfWriter() {
  if ( mSeries && mIsOpen ) {
    close();
  }
}

void XmfWriter::open(
  const xdm::FileSystemPath& path,
  xdm::Dataset::InitializeMode mode ) {
  mCurrentFilePath = path;
  mSeries = new TemporalCollection( path.pathString(), mode );
  mSeries->open();
  mIsOpen = true;
}

void XmfWriter::write( xdm::RefPtr< xdm::Item > item ) {
  using xdm::RefPtr;
  using xdmGrid::Grid;

  if ( !mSeries ) {
    XDM_THROW( xdmFormat::WriteError( "XDMF Series has not been opened." ) );
  }

  RefPtr< Grid > grid = xdm::dynamic_pointer_cast< Grid >( item );
  if ( !grid )  {
    XDM_THROW( xdmFormat::WriteError( "The XDMF plugin can write only grids." ) );
  }

  // Attach datasets to items that do not yet have them.
  xdmHdf::AttachHdfDatasetOperation attach( mCurrentFilePath.pathString() + ".h5", "unnamed" );
  grid->accept( attach );

  mSeries->writeGridMetadata( grid );
  mSeries->writeGridData( grid );
}

void XmfWriter::close() {
  mIsOpen = false;
  mSeries->close();
  mSeries.reset();
}

XDMF_NAMESPACE_END
