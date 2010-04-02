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
#define BOOST_TEST_MODULE ImplTreeBuilder
#include <boost/test/unit_test.hpp>

#include <xdmf/impl/TreeBuilder.hpp>

#include <xdm/Dataset.hpp>
#include <xdm/TypedStructuredArray.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace {

BOOST_AUTO_TEST_CASE( buildUniformDataItem ) {
  char const * const kXml = 
  "<DataItem Name='test' "
  "  ItemType='Uniform'"
  "  Dimensions='3 3'"
  "  NumberType='Float'"
  "  Precision='8'"
  "  Format='HDF'>"
  "  test.h5:/path/to/dataset"
  "</DataItem>";
  double resultData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  xmlDocPtr document = xmlParseDoc( reinterpret_cast< const xmlChar *>(kXml) );
  xmlNode * rootNode = xmlDocGetRootElement( document );

  xdmf::impl::TreeBuilder builder( document );
  xdm::RefPtr< xdm::UniformDataItem > item 
    = builder.buildUniformDataItem( rootNode );
  BOOST_REQUIRE( item );

  BOOST_CHECK_EQUAL( item->dataType(), xdm::primitiveType::kDouble );
  BOOST_CHECK_EQUAL( item->dataspace(), xdm::makeShape( 3, 3 ) );
  BOOST_REQUIRE( item->dataset() );
  BOOST_CHECK_EQUAL( item->dataset()->format(), "HDF" );

  xdm::RefPtr< xdmHdf::HdfDataset > dataset
    = xdm::dynamic_pointer_cast< xdmHdf::HdfDataset >( item->dataset() );
  BOOST_REQUIRE( dataset );

  BOOST_CHECK_EQUAL( dataset->file(), "test.h5" );
  xdmHdf::GroupPath path;
  path.push_back( "path" );
  path.push_back( "to" );
  BOOST_CHECK_EQUAL_COLLECTIONS(
    dataset->groupPath().begin(), dataset->groupPath().end(),
    path.begin(), path.end() );
  BOOST_CHECK_EQUAL( "dataset", dataset->dataset() );
  xmlFreeDoc( document );
}

} // namespace

