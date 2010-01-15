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
#define BOOST_TEST_MODULE TestDistributedItemCollectionProxy
#include <boost/test/unit_test.hpp>

#include <xdmComm/test/MpiTestFixture.hpp>

#include <xdmComm/DistributedItemCollectionProxy.hpp>

#include <xdm/CollectMetadataOperation.hpp>
#include <xdm/Item.hpp>
#include <xdm/ItemVisitor.hpp>

namespace {

xdmComm::test::MpiTestFixture globalFixture;

class ItemCollection : public xdm::Item {
public:
  std::list< xdm::RefPtr< xdm::Item > > mItems;

  XDM_META_ITEM( ItemCollection );

  virtual void traverse( xdm::ItemVisitor& iv ) {
    std::for_each( mItems.begin(), mItems.end(), xdm::ApplyVisitor( iv ) );
  }

  virtual void writeMetadata( xdm::XmlMetadataWrapper& metadata ) {
    xdm::Item::writeMetadata( metadata );
  }

};

class ProcessDescriptionItem : public xdm::Item {
public:

  XDM_META_ITEM( ProcessDescriptionItem );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& metadata ) {
    xdm::Item::writeMetadata( metadata );
    metadata.setAttribute( "rank", globalFixture.localRank() );
  }
};

BOOST_AUTO_TEST_CASE( distributedCollection ) {
  xdm::RefPtr< ItemCollection > item( new ItemCollection );
  item->mItems.push_back( new ProcessDescriptionItem );

  // put the proxy in place
  xdm::RefPtr< xdmComm::DistributedItemCollectionProxy > proxy(
    new xdmComm::DistributedItemCollectionProxy(
      item.get(),
      MPI_COMM_WORLD,
      1024 ) );

  xdm::CollectMetadataOperation collect;
  proxy->accept( collect );
  xdm::RefPtr< xdm::XmlObject > result = collect.result();

  xdm::RefPtr< xdm::XmlObject > answer( new xdm::XmlObject( "answer" ) );

  BOOST_CHECK_EQUAL( *answer, *result );
}

}
