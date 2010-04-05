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
#include <xdmExodus/Reader.hpp>

#include <xdmExodus/Blocks.hpp>
#include <xdmExodus/Helpers.hpp>
#include <xdmExodus/Sets.hpp>
#include <xdmExodus/Variable.hpp>

#include <xdmGrid/CollectionGrid.hpp>
#include <xdmGrid/Domain.hpp>
#include <xdmGrid/MultiArrayGeometry.hpp>
#include <xdmGrid/Time.hpp>

#include <xdm/FileSystem.hpp>
#include <xdm/ItemVisitor.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/VectorStructuredArray.hpp>

#include <cassert>
#include <string>
#include <vector>

XDM_EXODUS_NAMESPACE_BEGIN

namespace {

xdm::RefPtr< xdmGrid::Geometry > readGeometry( int exodusFileId, const ex_init_params& gridParameters ) {
  // Read the nodes. For Exodus, there is only one set of nodes per file.
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
    ex_get_coord( exodusFileId, xyzCoords[0]->data(), xyzCoords[1]->data(), xyzCoords[2]->data() ),
    "Could not read node coordinates from Exouds file." );

  return geom;
}

// An RAII class to handle an Exodus file. However, the behavior may be unpredictable if more than
// one of these exist in the same scope with the same file.
class ReadableExodusFile {
public:
  ReadableExodusFile( const xdm::FileSystemPath& exodusFileName ) :
    mFileId( -1 ) {

    if ( ! exists( exodusFileName ) ) {
      std::runtime_error( "The Exodus file does not exist at " + exodusFileName.pathString() );
    }

    // Open the file and get the id.
    float version;
    int doubleWordSize = sizeof( double );
    int storedDataWordSize = 0;
    mFileId = ex_open(
      exodusFileName.pathString().c_str(),
      EX_READ,
      &doubleWordSize,
      &storedDataWordSize,
      &version );
    if ( mFileId < 0 ) {
      throw std::runtime_error( "Could not open ExodusII file at " + exodusFileName.pathString() );
    }
  }

  ~ReadableExodusFile() {
    if ( mFileId >= 0 ) {
      try {
        ex_close( mFileId );
      } catch ( ... ) {
        // Should probably log this or something.
      }
    }
  }

  int id() const { return mFileId; }

private:
  int mFileId;
};

// This visitor can be used to traverse a tree that contains Variables. After the visitor
// is applied, all Variables contain the data from the requested time step. Dynamic casting
// is required here because the ItemVisitor does not know about xdmGrid or xdmExodus.
class UpdateVariableVisitor : public xdm::ItemVisitor {
public:
  UpdateVariableVisitor( int exodusFileId, std::size_t timeStep ) :
    mTimeStep( timeStep ),
    mFileId( exodusFileId ),
    mSomethingWasUpdated( false ) {}

  virtual ~UpdateVariableVisitor() {}

  virtual void apply( xdm::Item& item ) {
    Variable* variable = dynamic_cast< Variable* >( &item );
    if ( variable ) {
      variable->readTimeStep( mFileId, mTimeStep );
      mSomethingWasUpdated = true;
    }
  }

  bool somethingWasUpdated() const { return mSomethingWasUpdated; }

private:
  std::size_t mTimeStep;
  int mFileId;
  bool mSomethingWasUpdated;
};

// This visitor can be used to traverse a tree that contains xdmGrid::Grids. After the
// visitor is applied, all Grids will have the Time attached to them. Dynamic casting
// is required here because ItemVisitor does not know about xdmGrid.
class UpdateTimeVisitor : public xdm::ItemVisitor {
public:
  UpdateTimeVisitor( xdm::RefPtr< xdmGrid::Time > time ) :
    mTime( time ) {}

  virtual ~UpdateTimeVisitor() {}

  virtual void apply( xdm::Item& item ) {
    xdmGrid::Grid* grid = dynamic_cast< xdmGrid::Grid* >( &item );
    if ( grid ) {
      grid->setTime( mTime );
    }
  }

private:
  xdm::RefPtr< xdmGrid::Time > mTime;
};

struct ObjectGroupData {
  std::size_t numberOfObjects;
  std::vector< ExodusString > objectNames;
  std::vector< int > objectIds;
  std::size_t numberOfVariables;
  std::vector< ExodusString > variableNames;
  std::vector< int > variableTruthTable;

  ObjectGroupData() :
    numberOfObjects( 0 ),
    objectNames(),
    objectIds(),
    numberOfVariables( 0 ),
    variableNames(),
    variableTruthTable() {}
};

// Returns true if there were objects of this type, false if none exist.
bool readObjectGroupData(
  ObjectGroupData& group,
  int exodusFileId,
  std::size_t numberOfTimeSteps,
  std::size_t objectTypeIndex ) {

  // Get the number of instances of this type of object.
  EXODUS_CALL(
    ex_inquire( exodusFileId, kInquireObjectSizes[ objectTypeIndex ], &group.numberOfObjects, 0, 0 ),
    "The number of objects could not be determined." );

  if ( group.numberOfObjects < 1 ) {
    return false;
  }

  // Get the "ID" of all of the instances.
  group.objectIds.resize( group.numberOfObjects );
  EXODUS_CALL( ex_get_ids( exodusFileId, kObjectTypes[ objectTypeIndex ], &group.objectIds[0] ),
    "Could not read object IDs." );

  // Get the names of the instances, if they are named.
  group.objectNames.resize( group.numberOfObjects );
  char* objectNamesCharArray[ group.numberOfObjects ];
  vectorToCharStarArray( group.objectNames, objectNamesCharArray );
  EXODUS_CALL( ex_get_names( exodusFileId, kObjectTypes[ objectTypeIndex ], objectNamesCharArray ),
    "Could not read object names." );

  // For blocks and sets, get the variable information if there are time series.
  if ( numberOfTimeSteps > 0 &&
    ( objectIsBlock( objectTypeIndex ) || objectIsSet( objectTypeIndex ) ) ) {

    // Get the number of variables for this type of block or set.
    EXODUS_CALL(
      ex_get_var_param( exodusFileId, kObjectTypeChar[ objectTypeIndex ], &group.numberOfVariables ),
      "Could not read number of variables." );

    // The truth table gives a 1 or 0 denoting whether or not a particular variable is used
    // for each instance of this object type. For example, if there are 3 element blocks and
    // two variables for element blocks, then the truth table tells which variables are active
    // for each of the 3 blocks.
    if ( group.numberOfVariables > 0 ) {
      group.variableTruthTable.resize( group.numberOfVariables * group.numberOfObjects );
      EXODUS_CALL(
        ex_get_var_tab( exodusFileId, kObjectTypeChar[ objectTypeIndex ], group.numberOfObjects,
          group.numberOfVariables, &group.variableTruthTable[0] ),
        "Could not read the variable truth table." );

      // Get the names of the variables.
      group.variableNames.resize( group.numberOfVariables );
      char* variableNamesCharArray[ group.numberOfVariables ];
      vectorToCharStarArray( group.variableNames, variableNamesCharArray );
      EXODUS_CALL(
        ex_get_var_names( exodusFileId, kObjectTypeChar[ objectTypeIndex ],
          group.numberOfVariables, variableNamesCharArray ),
        "Could not read variable names." );
    }
  }
  return true;
}

// Fill an array with increasing ints and turn it into a data item.
xdm::RefPtr< xdm::UniformDataItem >
iotaArrayOfSizeT( const std::size_t& size, const std::size_t beginWith = 0 ) {
  xdm::RefPtr< xdm::VectorStructuredArray< std::size_t > > array(
    new xdm::VectorStructuredArray< std::size_t >( size ) );
  for ( std::size_t i = 0; i < size; ++i )   {
    (*array)[i] = i + beginWith;
  }
  xdm::DataShape<> datasetShape(1);
  datasetShape[0] = size;
  xdm::RefPtr< xdm::UniformDataItem > returnItem(
    xdm::makeRefPtr( new xdm::UniformDataItem( xdm::primitiveType::kLongUnsignedInt, datasetShape ) ) );
  returnItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( array ) ) );

  return returnItem;
}

} // anon namespace

ExodusReader::ExodusReader() {
}

ExodusReader::~ExodusReader() {
}

xdm::RefPtr< xdm::Item > ExodusReader::readItem( const xdm::FileSystemPath& path ) {

  // Open the file and get some info.
  ReadableExodusFile file( path );
  int fileId = file.id();

  // Get the mesh parameters and time steps.
  ex_init_params gridParameters;
  EXODUS_CALL( ex_get_init_ext( fileId, &gridParameters ), "Unable to read Exodus file parameters." );
  int numberOfTimeSteps = 0;
  EXODUS_CALL( ex_inquire( fileId, EX_INQ_TIME, &numberOfTimeSteps, 0, 0 ), "Unable to read"
    " number of time steps." );

  // The Item returned is an xdmGrid::Domain.
  xdm::RefPtr< xdmGrid::Domain > domain( new xdmGrid::Domain );
  xdm::RefPtr< xdmGrid::CollectionGrid > spatialCollection( new xdmGrid::CollectionGrid );
  domain->addGrid( spatialCollection );

  // The sets and maps refer to internal ordering of nodes, edges, faces, and elements. We
  // need to make a superset for each of these object types. Then, the sets and maps will
  // refer to their respective supersets.
  xdm::RefPtr< xdmGrid::UniformGrid > globalNodeSet( new xdmGrid::UniformGrid );
  std::vector< xdm::RefPtr< xdmGrid::ReferencingGrid > > globalSets( kNumberOfObjectTypes );
  globalSets[ EX_EDGE_BLOCK ] = xdm::makeRefPtr( new xdmGrid::ReferencingGrid );
  globalSets[ EX_FACE_BLOCK ] = xdm::makeRefPtr( new xdmGrid::ReferencingGrid );
  globalSets[ EX_ELEM_BLOCK ] = xdm::makeRefPtr( new xdmGrid::ReferencingGrid );
  globalSets[ EX_EDGE_SET ] = globalSets[ EX_EDGE_BLOCK ];
  globalSets[ EX_FACE_SET ] = globalSets[ EX_FACE_BLOCK ];
  globalSets[ EX_SIDE_SET ] = globalSets[ EX_ELEM_BLOCK ];
  globalSets[ EX_ELEM_SET ] = globalSets[ EX_ELEM_BLOCK ];
  globalSets[ EX_EDGE_MAP ] = globalSets[ EX_EDGE_BLOCK ];
  globalSets[ EX_FACE_MAP ] = globalSets[ EX_FACE_BLOCK ];
  globalSets[ EX_ELEM_MAP ] = globalSets[ EX_ELEM_BLOCK ];

  spatialCollection->appendChild( globalNodeSet );
  spatialCollection->appendChild( globalSets[ EX_EDGE_BLOCK ] );
  spatialCollection->appendChild( globalSets[ EX_FACE_BLOCK ] );
  spatialCollection->appendChild( globalSets[ EX_ELEM_BLOCK ] );

  //---------------NODES-------------------
  xdm::RefPtr< xdmGrid::Geometry > geom = readGeometry( fileId, gridParameters );
  xdm::RefPtr< xdm::UniformDataItem > nodeConn = iotaArrayOfSizeT( geom->numberOfNodes() );
  xdm::RefPtr< xdmGrid::UnstructuredTopology > nodeTopo( new xdmGrid::UnstructuredTopology );
  nodeTopo->setConnectivity( nodeConn );
  nodeTopo->setNumberOfCells( geom->numberOfNodes() );
  nodeTopo->setCellType( xdmGrid::CellType::Polyvertex );
  globalNodeSet->setGeometry( geom );
  globalNodeSet->setTopology( nodeTopo );

  //----------BLOCKS/SETS/MAPS-------------
  for ( std::size_t objectTypeIndex = 0; objectTypeIndex < kNumberOfObjectTypes; ++objectTypeIndex ) {

    ObjectGroupData group;
    if ( ! readObjectGroupData( group, fileId, numberOfTimeSteps, objectTypeIndex ) ) {
      continue;
    }

    // This is where we read the actual objects. These are blocks (edges, faces, elements), sets
    // (nodes, edges, faces, elements), or maps. They all contain integer arrays. These arrays
    // represent the connectivity info in the case of blocks, the collection info in the case
    // of sets, or the ordering info for edges/faces/elements in the case of maps.
    std::size_t globalOffset = 0;
    for ( std::size_t objectInstance = 0; objectInstance < group.numberOfObjects; ++objectInstance ) {

      if ( objectIsBlock( objectTypeIndex ) ) {

        xdm::RefPtr< Block > block = blockFactory( kObjectTypes[ objectTypeIndex ] );
        block->readFromFile(
          fileId,
          group.objectIds[ objectInstance ],
          group.objectNames[ objectInstance ].string(),
          geom,
          group.variableTruthTable.begin() + objectInstance * group.numberOfVariables,
          group.numberOfVariables,
          group.variableNames );

        block->setEntryGlobalOffset( globalOffset );
        globalOffset += block->numberOfEntries();

        spatialCollection->appendChild( block );

        globalSets[ objectTypeIndex ]->appendReferenceGrid( block );

      } else if ( objectIsSet( objectTypeIndex ) ) {

        // Since there can only be one distribution factor per entry, this number can be either
        // zero or numberOfEntries.
        int numberOfEntries = 0;
        int numberOfDistributionFactors;
        EXODUS_CALL(
          ex_get_set_param(
            fileId,
            kObjectTypes[ objectTypeIndex ],
            group.objectIds[ objectInstance ],
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
            group.objectIds[ objectInstance ],
            &setEntries[0],
            extraPtr ),
          "Could not read set." );

        // TODO: We need an XDM grid that holds a set, probably an XDMF SubSet Grid, which
        // we don't have yet.

        if ( numberOfDistributionFactors > 0 ) {
          std::vector< double > distributionFactors( numberOfDistributionFactors );
          EXODUS_CALL(
            ex_get_set_dist_fact(
              fileId,
              kObjectTypes[ objectTypeIndex ],
              group.objectIds[ objectInstance ] ),
            "Could not read set distribution factors." );
        }

      } else if ( objectIsMap( objectTypeIndex ) ) {

        int numberOfEntries = 0;
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
              group.objectIds[ objectInstance ],
              &map[0] ),
            "Could not read map." );
        }

        // TODO: We need an XDM grid that holds a map, probably an XDMF SubSet Grid, which
        // we don't have yet. This is even worse than sets, because these maps use Exodus
        // internal node numbering, and we do not have a way to store that info. Maybe create
        // a special Topology that holds a ref impl class that can point to elements/faces/edges
        // that exist in any of the other topologies.
      }

    } // object Index
  } // objectTypeIndex

  return domain;
}

std::size_t ExodusReader::numberOfTimeSteps( const xdm::FileSystemPath& path ) const {
  ReadableExodusFile file( path );
  int exodusFileId = file.id();
  int numberOfTimeSteps = 0;
  EXODUS_CALL( ex_inquire( exodusFileId, EX_INQ_TIME, &numberOfTimeSteps, 0, 0 ), "Unable to read"
    " number of time steps." );
  return numberOfTimeSteps;
}

bool ExodusReader::update(
  xdm::RefPtr< xdm::Item > item,
  const xdm::FileSystemPath& path,
  std::size_t timeStep ) {

  // First make sure we have enough time steps to process the update.
  if ( timeStep >= numberOfTimeSteps( path ) ) {
    return false;
  }

  // Open the Exodus file.
  ReadableExodusFile file( path );
  int exodusFileId = file.id();

  // Get the time at this step so that we can attach it to the grids.
  double timeAtThisStep = 0.0;
  EXODUS_CALL( ex_get_time( exodusFileId, (int)( timeStep + 1 ), &timeAtThisStep ),
    "Could not read time step." );
  xdm::RefPtr< xdmGrid::Time > time( new xdmGrid::Time );
  time->setValue( timeAtThisStep );

  // Attach the time to anything that is an xdmGrid::Grid.
  UpdateTimeVisitor gridVisitor( time );
  item->traverse( gridVisitor );

  // Read the variables at this time step.
  UpdateVariableVisitor varVisitor( exodusFileId, timeStep );
  item->traverse( varVisitor );

  return varVisitor.somethingWasUpdated();
}


XDM_EXODUS_NAMESPACE_END

