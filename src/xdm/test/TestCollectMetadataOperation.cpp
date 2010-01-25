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
#define BOOST_TEST_MODULE CollectMetadataOperation 
#include <boost/test/unit_test.hpp>

#include <xdm/CollectMetadataOperation.hpp>

#include <xdm/Item.hpp>

#include <algorithm>

namespace {

struct AggregateItem : xdm::Item {
  std::vector< xdm::RefPtr< Item > > mItems;
  void appendChild( xdm::RefPtr< xdm::Item > i ) { mItems.push_back( i ); }
  virtual void traverse( xdm::ItemVisitor& iv ) {
    std::for_each( mItems.begin(), mItems.end(), xdm::ApplyVisitor( iv ) );
  }
};

BOOST_AUTO_TEST_CASE( applyItem ) {
  xdm::CollectMetadataOperation op;
  xdm::Item i;

  i.accept( op );

  xdm::RefPtr< xdm::XmlObject > opResult = op.result();

  char const * const answer = 
    "<Item>\n"
    "</Item>\n";
  std::stringstream result;
  result << *opResult;
  BOOST_REQUIRE_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( applyAggregateItem ) {
  xdm::RefPtr< AggregateItem > item[4];
  item[0] = new AggregateItem;
  for ( int i = 1; i < 4; ++i ) {
    item[i] = new AggregateItem;
    item[i-1]->appendChild( item[i] );
  }

  xdm::CollectMetadataOperation op;
  item[0]->accept( op );
  xdm::RefPtr< xdm::XmlObject > opResult = op.result();

  char const * const answer = 
    "<Item>\n"
    "  <Item>\n"
    "    <Item>\n"
    "      <Item>\n"
    "      </Item>\n"
    "    </Item>\n"
    "  </Item>\n"
    "</Item>\n";
  std::stringstream result;
  result << *opResult;
  BOOST_REQUIRE_EQUAL( answer, result.str() );
}

} // namespace

