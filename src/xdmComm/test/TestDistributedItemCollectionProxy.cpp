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

#include <xdm/BinaryStreamOperations.hpp>
#include <xdm/CollectMetadataOperation.hpp>
#include <xdm/Item.hpp>
#include <xdm/ItemVisitor.hpp>

#include <algorithm>
#include <ostream>
#include <vector>

namespace {

xdmComm::test::MpiTestFixture globalFixture;

typedef std::pair< int, std::string > ProcessItemPair;

bool operator!=( const ProcessItemPair& lhs, const ProcessItemPair& rhs ) {
  return ( lhs.first != rhs.first ) || ( lhs.second != rhs.second );
}

std::ostream& operator<<( std::ostream& ostr, const ProcessItemPair& pair ) {
  ostr << pair.first << " -- " << pair.second;
  return ostr;
}

class ProcessReportVisitor : public xdm::ItemVisitor {
public:
  std::vector< ProcessItemPair > mVisitedObjects;

  virtual void apply( xdm::Item& item ) {
    mVisitedObjects.push_back(
      std::make_pair( globalFixture.localRank(), item.className() ) );
  }

  virtual void captureState( xdm::BinaryOStream& ostr ) {
    ostr << mVisitedObjects.size();
    std::for_each( mVisitedObjects.begin(), mVisitedObjects.end(),
      xdm::OutputObject< ProcessItemPair >( ostr ) );
  }

  virtual void restoreState( xdm::BinaryIStream& istr ) {
    std::vector< ProcessItemPair >::size_type size;
    istr >> size;
    for ( size_t i = 0; i < size; i++ ) {
      ProcessItemPair element;
      istr >> element;
      mVisitedObjects.push_back( element );
    }
  }
};

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

/*BOOST_AUTO_TEST_CASE( distributedCollection ) {
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
}*/

BOOST_AUTO_TEST_CASE( visitsAll ) {
  xdm::RefPtr< ItemCollection > item( new ItemCollection );
  item->mItems.push_back( new ProcessDescriptionItem );

  xdm::RefPtr< xdmComm::DistributedItemCollectionProxy > proxy(
    new xdmComm::DistributedItemCollectionProxy(
      item.get(),
      MPI_COMM_WORLD,
      1024 ) );

  ProcessReportVisitor visitor;
  proxy->accept( visitor );

  if ( globalFixture.localRank() == 0 ) {

    // Check that the visitor visited 1 + process count objects (parent and local,
    // plus one for each remote).
    BOOST_CHECK_EQUAL( visitor.mVisitedObjects.size(),
                       globalFixture.processes() + 1 );

    std::vector< ProcessItemPair > answer;
    answer.push_back( std::make_pair( 0, std::string( "ItemCollection" ) ) );
    for ( int i = 0; i < globalFixture.processes(); i++ ) {
      answer.push_back( std::make_pair( i, std::string( "ProcessDescriptionItem" ) ) );
    }

    BOOST_CHECK_EQUAL_COLLECTIONS(
        answer.begin(), answer.end(),
        visitor.mVisitedObjects.begin(), visitor.mVisitedObjects.end() );
  }
}

} // namespace
