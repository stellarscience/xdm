//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
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
#include <xdmExodus/ExodusReader.hpp>
#include <xdmExodus/ExodusConstants.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Domain.hpp>
#include <xdmGrid/MultiArrayGeometry.hpp>
#include <xdmGrid/Polyvertex.hpp>
#include <xdmGrid/UniformGrid.hpp>
#include <xdmGrid/UnstructuredTopology.hpp>
#include <xdmGrid/UnstructuredTopologyConventions.hpp>

#include <xdm/ArrayAdapter.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/VectorStructuredArray.hpp>

#include <algorithm>
#include <functional>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

XDM_EXODUS_NAMESPACE_BEGIN

namespace {

// Helpers that create a UniformDataItem from a StructuredArray. This is done frequently.
// First version takes one dimension.
xdm::RefPtr< xdm::UniformDataItem > makeDataItem(
  xdm::RefPtr< xdm::StructuredArray > vector,
  xdm::primitiveType::Value primType,
  std::size_t firstExtent ) {

  xdm::RefPtr< xdm::UniformDataItem > dataItem(
    new xdm::UniformDataItem( primType, xdm::makeShape( firstExtent ) ) );
  dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( vector ) ) );
}

// Second version takes two dimensions.
xdm::RefPtr< xdm::UniformDataItem > makeDataItem(
  xdm::RefPtr< xdm::StructuredArray > vector,
  xdm::primitiveType::Value primType,
  std::size_t firstExtent,
  std::size_t secondExtent ) {

  xdm::RefPtr< xdm::UniformDataItem > dataItem(
    new xdm::UniformDataItem( primType, xdm::makeShape( firstExtent, secondExtent ) ) );
  dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( vector ) ) );
}

xdmGrid::CellType::Type lookupCellType( std::size_t nodesPerCell, const std::string& cellName ) {
  // Make a map that takes the number of nodes and the Exodus name and gives a CellType.
  typedef std::map< std::string, xdmGrid::CellType::Type > NameMap;
  typedef std::map< std::size_t, NameMap > NumberOfNodesMap;
  static NumberOfNodesMap nodeMap;
  if ( nodeMap.empty() ) {
    nodeMap[ 1 ][ "CIRCLE" ] = xdmGrid::CellType::Circle;
    nodeMap[ 1 ][ "SPHERE" ] = xdmGrid::CellType::Sphere;
    nodeMap[ 2 ][ "TRUSS" ] = xdmGrid::CellType::Beam;
    nodeMap[ 2 ][ "BEAM" ] = xdmGrid::CellType::Beam;
    nodeMap[ 2 ][ "SHELL(2D)" ] = xdmGrid::CellType::Beam;
    nodeMap[ 3 ][ "TRIANGLE" ] = xdmGrid::CellType::Triangle;
    nodeMap[ 3 ][ "TRUSS" ] = xdmGrid::CellType::QuadraticBeam;
    nodeMap[ 3 ][ "BEAM" ] = xdmGrid::CellType::QuadraticBeam;
    nodeMap[ 3 ][ "SHELL(2D)" ] = xdmGrid::CellType::QuadraticBeam;
    nodeMap[ 4 ][ "QUAD" ] = xdmGrid::CellType::Quad;
    nodeMap[ 4 ][ "SHELL(3D)" ] = xdmGrid::CellType::Quad;
    nodeMap[ 4 ][ "TETRA" ] = xdmGrid::CellType::Tetra;
    nodeMap[ 5 ][ "PYRAMID" ] = xdmGrid::CellType::Pyramid;
    nodeMap[ 6 ][ "WEDGE" ] = xdmGrid::CellType::Wedge;
    nodeMap[ 7 ][ "TRIANGLE" ] = xdmGrid::CellType::QuadraticTriangle;
    nodeMap[ 8 ][ "HEX" ] = xdmGrid::CellType::Hex;
    nodeMap[ 9 ][ "QUAD" ] = xdmGrid::CellType::QuadraticQuad;
    nodeMap[ 9 ][ "SHELL(3D)" ] = xdmGrid::CellType::QuadraticQuad;
    nodeMap[ 11 ][ "TETRA" ] = xdmGrid::CellType::QuadraticTetra;
    nodeMap[ 13 ][ "PYRAMID" ] = xdmGrid::CellType::QuadraticPyramid;
    nodeMap[ 16 ][ "WEDGE" ] = xdmGrid::CellType::QuadraticWedge;
    nodeMap[ 21 ][ "HEX" ] = xdmGrid::CellType::QuadraticHex;
  }

  // Find the correct CellType by first matching number of nodes and then matching the
  // Exodus name.
  NumberOfNodesMap::const_iterator matchingNodes = nodeMap.find( nodesPerCell );
  if ( matchingNodes != nodeMap.end() ) {
    const NameMap& matchingNames = matchingNodes->second;
    NameMap::const_iterator matchingName = matchingNames.find( cellName );
    if ( matchingName != matchingNames.end() ) {
      return matchingName->second;
    }
  }

  std::stringstream ss;
  ss << "A cell type was not found for Exodus name " << cellName << " with number of"
    " nodes per cell.";
  throw std::runtime_error( ss.str() );
}

} // anon namespace

ExodusReader::ExodusReader() {
}

ExodusReader::~ExodusReader() {
}

xdm::RefPtr< xdm::Item > ExodusReader::readItem( const std::string& filename ) {
  // Open the file and get some info.
  float version;
  int doubleWordSize = sizeof( double );
  int storedDataWordSize = 0;
  int fileId = ex_open(
    filename.c_str(),
    EX_READ,
    &doubleWordSize,
    &storedDataWordSize,
    &version );
  if ( fileId < 0 ) {
    throw std::runtime_error( "Could not open ExodusII file at " + filename );
  }

  // Get the mesh parameters and time steps.
  ex_init_params gridParameters;
  EXODUS_CALL( ex_get_init_ext( fileId, &gridParameters ), "Unable to read Exodus file parameters." );
  int numberOfTimeSteps;
  EXODUS_CALL( ex_inquire( fileId, EX_INQ_TIME, &numberOfTimeSteps, 0, 0 ), "Unable to read"
    " number of time steps." );
  std::vector< double > timeSteps( numberOfTimeSteps );
  if ( numberOfTimeSteps > 0 ) {
    EXODUS_CALL( ex_get_all_times( fileId, &timeSteps[0] ), "Could not read time steps." );
  }

  //---------------NODES-------------------
  // Read the nodes. For Exodus, there is only ever one set of nodes that all element blocks
  // refer to.
  std::vector< xdm::RefPtr< xdm::VectorStructuredArray< double > > > xyzCoords( 3 );
  xdm::RefPtr< xdmGrid::MultiArrayGeometry > geom(
    new xdmGrid::MultiArrayGeometry( gridParameters.num_dim ) );
  for( std::size_t dim = 0; dim < gridParameters.num_dim; ++dim ) {
    xyzCoords[ dim ] = new xdm::VectorStructuredArray< double >( gridParameters.num_nodes );
    xdm::RefPtr< xdm::UniformDataItem > dataItem = makeDataItem(
      xyzCoords[ dim ], xdm::primitiveType::kDouble, gridParameters.num_nodes );
    geom->setCoordinateValues( dim, dataItem );
  }
  EXODUS_CALL(
    ex_get_coord( fileId, xyzCoords[0]->data(), xyzCoords[1]->data(), xyzCoords[2]->data() ),
    "Could not read node coordinates from Exouds file." );

  for ( std::size_t objectTypeIndex = 0; objectTypeIndex < kNumberOfObjectTypes; ++objectTypeIndex ) {

    // Get the number of instances of this type of object.
    int numberOfObjectInstances;
    EXODUS_CALL(
      ex_inquire( fileId, kInquireObjectSizes[ objectTypeIndex ], &numberOfObjectInstances, 0, 0 ),
      "The number of objects could not be determined." );

    if ( ! numberOfObjectInstances ) {
      // Skip this object type if there are no instances.
      continue;
    }

    // Get the "ID" of all of the instances.
    std::vector< int > objectIds( numberOfObjectInstances );
    EXODUS_CALL( ex_get_ids( fileId, kObjectTypes[ objectTypeIndex ], &objectIds[0] ),
      "Could not read object IDs." );

    // Get the names of the instances, if they are named.
    std::vector< ExodusString > objectNames( numberOfObjectInstances );
    char* objectNamesCharArray[ numberOfObjectInstances ];
    vectorToCharStarArray( objectNames, objectNamesCharStar );
    EXODUS_CALL( ex_get_names( fileId, kObjectTypes[ objectTypeIndex ], objectNamesCharArray ),
      "Could not read object names." );

    // For blocks and sets, get the variable information if there are time series.
    int numberOfVariables = 0;
    std::vector< int > variableTruthTable;
    std::vector< ExodusString > variableNames;
    if ( numberOfTimeSteps > 0 &&
      ( objectIsBlock( objectTypeIndex ) || objectIsSet( objectTypeIndex ) ) ) {

      // Get the number of variables for this type of block or set.
      EXODUS_CALL(
        ex_get_var_param( fileId, kObjectTypeChar[ objectTypeIndex ], &numberOfVariables ),
        "Could not read number of variables." );

      // The truth table gives a 1 or 0 denoting whether or not a particular variable is used
      // for each instance of this object type. For example, if there are 3 element blocks and
      // two variables for element blocks, then the truth table tells which variables are active
      // for each of the 3 blocks.
      if ( numberOfVariables > 0 ) {
        variableTruthTable.resize( numberOfVariables * numberOfObjectInstances );
        EXODUS_CALL(
          ex_get_var_tab( fileId, kObjectTypeChar[ objectTypeIndex ], numberOfObjectInstances,
            numberOfVariables, &variableTruthTable[0] ),
          "Could not read the variable truth table." );

        // Get the names of the variables.
        variableNames.resize( numberOfVariables );
        char* variableNamesCharArray[ numberOfVariables ];
        vectorToCharStarArray( variableNames, variableNamesCharArray );
        EXODUS_CALL(
          ex_get_var_names( fileId, kObjectTypeChar[ objectTypeIndex ],
            numberOfVariables, variableNames ),
          "Could not read variable names." );
      }
    }

    // This is where we read the actual objects. These are blocks (edges, faces, elements), sets
    // (nodes, edges, faces, elements), or maps. They all contain integer arrays. These arrays
    // represent the connectivity info in the case of blocks, the collection info in the case
    // of sets, or the ordering info for edges/faces/elements in the case of maps.
    for ( std::size_t objectInstance = 0; objectInstance < numberOfObjectInstances; ++objectInstance ) {
      int numberOfEntries = 0;

      if ( objectIsBlock( objectTypeIndex ) ) {

        int nodesPerEntry;
        int edgesPerEntry;
        int facesPerEntry;
        int attributesPerEntry;
        ExodusString entryType; // e.g. TET4, HEX8
        EXODUS_CALL(
          ex_get_block(
            fileId,
            kObjectTypes[ objectTypeIndex ],
            objectIds[ objectInstance ],
            entryType.ptr(),
            &numberOfEntries,
            &nodesPerEntry,
            &edgesPerEntry,
            &facesPerEntry,
            &attributesPerEntry ),
          "Could not read block params." );

        std::vector< int > nodeConnections( numberOfEntries * nodesPerEntry );
        std::vector< int > edgeConnections( numberOfEntries * edgesPerEntry );
        std::vector< int > faceConnections( numberOfEntries * facesPerEntry );
        EXODUS_CALL(
          ex_get_conn(
            fileId,
            kObjectTypes[ objectTypeIndex ],
            objectIds[ objectInstance ],
            &nodeConnections[0],
            &edgeConnections[0],
            &faceConnections[0] ),
          "Could not read connectivity." );

        // Read the attributes. For element blocks, these are cell-centered values, but there
        // is no way to know here whether they should be interpreted to be scalars,
        // vectors, matrices, etc, except by maybe checking their names. For Exodus, all
        // attributes (and variables) are floating point, so this routine just reads them
        // all into a single data item.
        if ( attributesPerEntry > 0 ) {
          std::vector< ExodusString > attributeNames( attributesPerEntry );
          char* attributeNamesCharArray[ attributesPerEntry ];
          vectorToCharStarArray( attributeNames, attributeNamesCharArray );
          EXODUS_CALL(
            ex_get_attr_names(
              fileId,
              kObjectTypes[ objectTypeInstance ],
              objectIds[ objectInstance ],
              attributeNamesCharArray ),
            "Could not read attribute names." );

          std::vector< double > attributeValues( attributesPerEntry * numberOfEntries );
          EXODUS_CALL(
            ex_get_attr(
              fileId,
              kObjectTypes[ objectTypeIndex ],
              objectIds[ objectInstance ],
              &attributeValues[0] ),
            "Could not read attribute values." );
        }

      } else if ( objectIsSet( objectTypeIndex ) ) {

        // Since there can only be one distribution factor per entry, this number can be either
        // zero or numberOfEntries.
        int numberOfDistributionFactors;
        EXODUS_CALL(
          ex_get_set_param(
            fileId,
            kObjectTypes[ objectTypeIndex ],
            objectIds[ objectInstance ],
            &numberOfEntries,
            &numberOfDistributionFactors ),
          "Could not read set parameters." );

        std::vector< int > setEntries( numberOfEntries );
        // setExtra contains an int for each entry. The use of this int is only specified for
        // side sets, where the entry refers to the particular element and the extra int refers
        // to a specific side in that element. However, edge and face sets can also use this
        // extra int to specify things like direction (-1/+1).
        std::vector< int > setExtra;
        int* extraPtr = 0;
        if ( kObjectTypes[ objectTypeIndex ] != EX_NODE_SET &&
             kObjectTypes[ objectTypeIndex ] != EX_ELEM_SET ) {
          setExtra.resize( numberOfEntries );
          extraPtr = &setExtra[0];
        }
        EXODUS_CALL(
          ex_get_set(
            fileId,
            kObjectTypes[ objectTypeIndex ],
            objectIds[ objectInstance ],
            &setEntries[0],
            extraPtr ),
          "Could not read set." );

        if ( numberOfDistributionFactors > 0 ) {
          std::vector< double > distributionFactors( numberOfDistributionFactors );
          EXODUS_CALL(
            ex_get_set_dist_fact(
              fileId,
              kObjectTypes[ objectTypeIndex ],
              objectIds[ objectInstance ] ),
            "Could not read set distribution factors." );
        }

      } else if ( objectIsMap( objectTypeIndex ) ) {

        switch ( kObjectTypes[ objectTypeIndex ] ) {
          case EX_NODE_MAP:
            numberOfEntries = gridParameters.num_nodes;
            break;
          case EX_EDGE_MAP:
            numberOfEntries = gridParameters.num_edge;
            break;
          case EX_FACE_MAP:
            numberOfEntries = gridParameters.num_face;
            break;
          case EX_ELEM_MAP:
            numberOfEntries = gridParameters.num_elem;
            break;
        }
        if ( numberOfEntries > 0 ) {
          std::vector< int > map( numberOfEntries );
          EXODUS_CALL(
            ex_get_num_map(
              fileId,
              kObjectTypes[ objectTypeIndex ],
              objectIds[ objectInstance ],
              &map[0] ),
            "Could not read map." );
        }
      }

      // Read the results variables.
      if ( numberOfVariables > 0 &&
        ( objectIsBlock( objectTypeIndex ) || objectIsSet( objectTypeIndex ) ) ) {

        std::vector< double > variable( numberOfEntries );
        for ( std::size_t variableIndex = 0; variableIndex < numberOfVariables; ++variableIndex ) {
          if ( variableTruthTable[ numberOfVariables * objectInstance + variableIndex ] == 0 ) {
            continue;
          }

          for ( std::size_t timeStep = 1; timeStep <= numberOfTimesteps; ++timeStep ) {
            EXODUS_CALL(
              ex_get_var(
                fileId,
                timeStep,
                kObjectTypes[ objectTypeIndex ],
                variableIndex + 1,
                objectIds[ objectInstance ],
                numberOfEntries,
                &variable[0] ),
              "Could not read variable values." );
          }
        }
      }

    } // object Index
  } // objectTypeIndex

  EXODUS_CALL( ex_close( fileId ), "Unable to close Exodus file." );

  return 0;
}





























  // Open the file and get some info.
  float version;
  int doubleWordSize = sizeof( double );
  int storedDataWordSize = 0;
  int fileId = ex_open(
    filename.c_str(),
    EX_READ,
    &doubleWordSize,
    &storedDataWordSize,
    &version );
  if ( fileId < 0 ) {
    throw std::runtime_error( "Could not open ExodusII filename: " + filename );
  }

  ex_init_params gridParameters;
  EXODUS_CALL(  ex_get_init_ext( fileId, &gridParameters ), "ex_get_init_ext returned an error." );

  //---------------NODES-------------------
  // Read the nodes. For Exodus, there is only ever one set of nodes that all element blocks
  // refer to.
  std::vector< xdm::RefPtr< xdm::VectorStructuredArray< double > > > xyzCoords( 3 );
  xdm::RefPtr< xdmGrid::MultiArrayGeometry > geom(
    new xdmGrid::MultiArrayGeometry( gridParameters.num_dim ) );
  for( std::size_t dim = 0; dim < gridParameters.num_dim; ++dim ) {
    xyzCoords[ dim ] = new xdm::VectorStructuredArray< double >( gridParameters.num_nodes );
    xdm::RefPtr< xdm::UniformDataItem > dataItem = makeDataItem(
      xyzCoords[ dim ], xdm::primitiveType::kDouble, gridParameters.num_nodes );
    geom->setCoordinateValues( dim, dataItem );
  }
  EXODUS_CALL(
    ex_get_coord( fileId, xyzCoords[0]->data(), xyzCoords[1]->data(), xyzCoords[2]->data() ),
    "ex_get_coord returned an error." );

  //-----------ELEMENT BLOCKS--------------
  // Read the each element block into a separate topology and add it to the domain.
  xdm::RefPtr< xdmGrid::Domain > domain( new xdmGrid::Domain );
  std::vector< int > blockIds( gridParameters.num_elem_blk );
  EXODUS_CALL( ex_get_ids( fileId, EX_ELEM_BLOCK, &blockIds[0] ), "ex_get_ids returned an error." );
  for( std::vector< int >::const_iterator blockId = blockIds.begin(); blockId != blockIds.end();
    ++blockId ) {
    ExodusString elementType, blockName;
    int numberOfElements, numberOfNodesPerElement, numberOfAttributesPerElement;
    int numberOfEdgesPerElement, numberOfFacesPerElement; // unused here
    EXODUS_CALL( ex_get_block(
      fileId,
      EX_ELEM_BLOCK,
      *blockId,
      elementType.ptr(),
      &numberOfElements,
      &numberOfNodesPerElement,
      &numberOfEdgesPerElement,
      &numberOfFacesPerElement,
      &numberOfAttributesPerElement ), "ex_get_block returned an error." );
    EXODUS_CALL( ex_get_name( fileId, EX_ELEM_BLOCK, *blockId, blockName.ptr() ),
      "ex_get_name returned an error." );
    std::vector< int > intConnectivity( numberOfElements * numberOfNodesPerElement );
    EXODUS_CALL( ex_get_conn( fileId, EX_ELEM_BLOCK, *blockId, &intConnectivity[0], 0, 0 ),
      "ex_get_conn returned an error." );

    // Exodus node numbering starts at 1, so change that to 0. Also, the connections have
    // to be std::size_t for xdm.
    xdm::RefPtr< xdm::VectorStructuredArray< std::size_t > > connectivity(
      new xdm::VectorStructuredArray< std::size_t >( intConnectivity.size() ) );
    std::transform( intConnectivity.begin(), intConnectivity.end(), connectivity->begin(),
      std::bind2nd( std::minus< std::size_t >(), 1 ) );

    // Turn the connectivity into a data item and give it to a topology along with the
    // geometry.
    xdm::RefPtr< xdm::UniformDataItem > dataItem = makeDataItem(
      connectivity, xdm::primitiveType::kLongUnsignedInt, numberOfElements, numberOfNodesPerElement );
    xdm::RefPtr< xdmGrid::UnstructuredTopology > elementBlock(
      new xdmGrid::UnstructuredTopology() );
    elementBlock->setConnectivity( dataItem );
    elementBlock->setNumberOfCells( numberOfElements );
    elementBlock->setCellType( lookupCellType( numberOfNodesPerElement, elementType.string() ) );
    elementBlock->setNodeOrdering( xdmGrid::NodeOrderingConvention::ExodusII );
    elementBlock->setName( blockName.string() );

    // There is a UniformGrid for each topology... however, they all refer to the same
    // Geometry.
    xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid() );
    grid->setGeometry( geom );
    grid->setTopology( elementBlock );

    // Read the attributes. For element blocks, these are cell-centered values, but there
    // is no way to know here whether they should be interpreted to be scalars,
    // vectors, matrices, etc.
    ExodusString attrNames[ numberOfAttributesPerElement ];
    char* attrNamesPtr[ numberOfAttributesPerElement ];
    std::transform( attrNames, attrNames + numberOfAttributesPerElement, attrNamesPtr,
      std::mem_fun_ref( &ExodusString::ptr ) );
    EXODUS_CALL( ex_get_attr_names( fileId, EX_ELEM_BLOCK, *blockId, attrNamesPtr ),
      "ex_get_attr_names returned an error." );

    for ( std::size_t attrIndex = 0; attrIndex < numberOfAttributesPerElement; ++attrIndex ) {
      xdm::RefPtr< xdm::VectorStructuredArray< double > > attribute(
        new xdm::VectorStructuredArray< double >( numberOfElements ) );
      EXODUS_CALL( ex_get_one_attr( fileId, EX_ELEM_BLOCK, *blockId, attrIndex + 1,
        attribute->data() ), "ex_get_one_attr returned an error." );
      xdm::RefPtr< xdm::UniformDataItem > dataItem = makeDataItem(
        attribute, xdm::primitiveType::kDouble, numberOfElements );
      xdm::RefPtr< xdmGrid::Attribute > attr(
        new xdmGrid::Attribute( xdmGrid::Attribute::kScalar, xdmGrid::Attribute::kCell ) );
      attr->setName( attrNames[ attrIndex ].string() );
      grid->addAttribute( attr );
    }

    domain->addGrid( grid );
  }

  //-------------NODE SETS-----------------
  // Read node sets into polyvertex topologies
  std::vector< int > nodeSetIds( gridParameters.num_node_sets );
  EXODUS_CALL( ex_get_ids( fileId, EX_NODE_SET, &nodeSetIds[0] ), "ex_get_ids returned an error." );
  for( std::vector< int >::const_iterator nodeSetId = nodeSetIds.begin();
    nodeSetId != nodeSetIds.end(); ++nodeSetId ) {
    int numberOfNodes, numberOfDistributionFactors;
    EXODUS_CALL( ex_get_set_param(
      fileId,
      EX_NODE_SET,
      *nodeSetId,
      &numberOfNodes,
      &numberOfDistributionFactors ), "ex_get_set_param returned an error." );
    std::vector< int > intNodeList( numberOfNodes );
    EXODUS_CALL( ex_get_set( fileId, EX_NODE_SET, *nodeSetId, &intNodeList[0], NULL ),
      "ex_get_set returned an error." );

    // Exodus node numbering starts at 1, so change that to 0. Also, the node indices have
    // to be std::size_t for xdm.
    xdm::RefPtr< xdm::VectorStructuredArray< std::size_t > > nodeList(
      new xdm::VectorStructuredArray< std::size_t >( intNodeList.size() ) );
    std::transform( intNodeList.begin(), intNodeList.end(), nodeList->begin(),
      std::bind2nd( std::minus< std::size_t >(), 1 ) );

    // Turn the node list into a data item and give it to a polyvertex topology along with the
    // geometry that everything else uses.
    xdm::RefPtr< xdm::UniformDataItem > dataItem = makeDataItem(
      nodeList, xdm::primitiveType::kLongUnsignedInt, numberOfNodes );
    xdm::RefPtr< xdmGrid::Polyvertex > polyvertex( new xdmGrid::Polyvertex() );
    polyvertex->setConnectivity( dataItem );
    polyvertex->setNumberOfCells( numberOfNodes );

    xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid() );
    grid->setGeometry( geom );
    grid->setTopology( polyvertex );
    domain->addGrid( grid );
  }



  // TODO: do something with side sets

  // TODO: do something with node number map... maybe read it into polyvertex

  // TODO: do something with element number map

  // TODO: do something with optimized element order map

  // TODO: take care of results

  // TODO: maybe read distribution factors into an attribute

  ex_close( fileId );

  return domain;
}

XDM_EXODUS_NAMESPACE_END

