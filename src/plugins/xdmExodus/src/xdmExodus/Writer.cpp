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
#include <xdmExodus/Blocks.hpp>
#include <xdmExodus/Helpers.hpp>
#include <xdmExodus/Variable.hpp>
#include <xdmExodus/Writer.hpp>

#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/Time.hpp>
#include <xdmGrid/Topology.hpp>
#include <xdmGrid/UnstructuredTopologyConventions.hpp>

#include <xdm/FileSystem.hpp>
#include <xdm/Item.hpp>
#include <xdm/ItemVisitor.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/TypedStructuredArray.hpp>

#include <map>
#include <string>
#include <vector>

XDM_EXODUS_NAMESPACE_BEGIN

namespace {

// An RAII class to handle an Exodus file. However, the behavior may be unpredictable if more than
// one of these exist in the same scope with the same file.
class WritableExodusFile {
public:
  WritableExodusFile( const xdm::FileSystemPath& exodusFileName ) :
    mFileId( -1 ) {

    // Open the file and get the id.
    int doubleWordSize = sizeof( double );
    int storedDataWordSize = sizeof( double );
    mFileId = ex_create(
      exodusFileName.pathString().c_str(),
      EX_CLOBBER,
      &doubleWordSize,
      &storedDataWordSize );
    if ( mFileId < 0 ) {
      throw std::runtime_error( "Could not create ExodusII file at " + exodusFileName.pathString() );
    }
  }

  ~WritableExodusFile() {
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

typedef std::map< int, std::string > VariableNameMap;

class VariableNamesVisitor : public xdm::ItemVisitor {
public:
  virtual void apply( xdm::Item& item ) {
    Variable* variable = dynamic_cast< Variable* >( &item );
    if ( variable ) {
      mNames[ variable->id() ] = variable->name();
    }
  }

  const VariableNameMap& names() const { return mNames; }

private:
  VariableNameMap mNames;
};

// T is expected to be a type inheriting from Object so that we can call numberOfEntries() and
// count variables.
template< typename T >
class GatherExodusObjectsVisitor : public xdm::ItemVisitor {
public:
  GatherExodusObjectsVisitor() : mEntryCount( 0 ) {}

  virtual void apply( xdm::Item& item ) {
    T* dynamicCastResult = dynamic_cast< T* >( &item );
    if ( dynamicCastResult ) {
      mPointers.push_back( dynamicCastResult );
      mEntryCount += dynamicCastResult->numberOfEntries();
      VariableNamesVisitor countVarVisitor;
      dynamicCastResult->traverse( countVarVisitor );
      mNames.insert( countVarVisitor.names().begin(), countVarVisitor.names().end() );
    }
  }

  std::vector< T* >& pointers() { return mPointers; }
  std::size_t totalNumberOfEntries() const { return mEntryCount; }
  const VariableNameMap& names() const { return mNames; }

  int maxVariableId() const {
    if ( mNames.size() > 0 ) {
      return mNames.rbegin()->first;
    } else {
      return 0;
    }
  }

private:
  VariableNameMap mNames;
  std::vector< T* > mPointers;
  std::size_t mEntryCount;
};

// This is expected to only be called on UniformGrids or other classes that contain Attributes.
// The visitor collects attributes, distinguishing between Exodus "attributes" and Variables.
class AttributeVariableVisitor : public xdm::ItemVisitor {
public:
  virtual void apply( xdm::Item& item ) {
    xdmGrid::Attribute* attribute = dynamic_cast< xdmGrid::Attribute* >( &item );
    if ( attribute ) {
      Variable* variable = dynamic_cast< Variable* >( attribute );
      if ( variable ) {
        mVariables.push_back( variable );
      } else {
        mAttributes.push_back( attribute );
      }
    }
  }

  std::vector< xdmGrid::Attribute* >& attributes() { return mAttributes; }
  std::vector< Variable* >& variables() { return mVariables; }


private:
  std::vector< xdmGrid::Attribute* > mAttributes;
  std::vector< Variable* > mVariables;
};

} // anon namespace

ExodusWriter::ExodusWriter() {
}

ExodusWriter::~ExodusWriter() {
}

void ExodusWriter::writeItem( xdm::RefPtr< xdm::Item > item, const xdm::FileSystemPath& path ) {

  WritableExodusFile file( path );

  std::size_t timeStep = 0;

  ex_init_params meshParams;
  std::strcpy( meshParams.title, item->name().substr( 0, MAX_LINE_LENGTH + 1 ).c_str() );

  // First make up collections of pointers to everything that we know about. For now,
  // that is just Blocks.
  GatherExodusObjectsVisitor< EdgeBlock > edgeBlockGatherVisitor;
  GatherExodusObjectsVisitor< FaceBlock > faceBlockGatherVisitor;
  GatherExodusObjectsVisitor< ElementBlock > elementBlockGatherVisitor;
  item->traverse( edgeBlockGatherVisitor );
  item->traverse( faceBlockGatherVisitor );
  item->traverse( elementBlockGatherVisitor );
  meshParams.num_edge_blk = edgeBlockGatherVisitor.pointers().size();
  meshParams.num_face_blk = faceBlockGatherVisitor.pointers().size();
  meshParams.num_elem_blk = elementBlockGatherVisitor.pointers().size();

  if ( meshParams.num_edge_blk + meshParams.num_face_blk + meshParams.num_elem_blk < 1 ) {
    // There is nothing to write.
    return;
  }

  // There is only one Geometry (one unique set of nodes). To find it, we just need to find
  // the first one.
  xdmGrid::Geometry* geom;
  if ( meshParams.num_edge_blk > 0 ) {
    geom = edgeBlockGatherVisitor.pointers().front()->geometry().get();
  } else if ( meshParams.num_face_blk > 0 ) {
    geom = faceBlockGatherVisitor.pointers().front()->geometry().get();
  } else {
    geom = elementBlockGatherVisitor.pointers().front()->geometry().get();
  }
  meshParams.num_dim = geom->dimension();
  meshParams.num_nodes = geom->numberOfNodes();
  std::vector< void* > geomPtrs( 3, NULL );
  for ( std::size_t dim = 0; dim < meshParams.num_dim; ++dim ) {
    geomPtrs[ dim ] = geom->child( dim )->data()->array()->data();
  }
  EXODUS_CALL( ex_put_coord( file.id(), geomPtrs[0], geomPtrs[1], geomPtrs[2] ),
    "Unable to write coordinates." );
  std::vector< ExodusString > coordNames;
  coordNames.push_back( "X" );
  coordNames.push_back( "Y" );
  coordNames.push_back( "Z" );
  char* coordNamesCharArray[ 3 ];
  vectorToCharStarArray( coordNames, coordNamesCharArray );
  EXODUS_CALL( ex_put_coord_names( file.id(), coordNamesCharArray ),
    "Unable to write coordinate names." );

  // Count the entries in the blocks.
  meshParams.num_edge = edgeBlockGatherVisitor.totalNumberOfEntries();
  meshParams.num_face = faceBlockGatherVisitor.totalNumberOfEntries();
  meshParams.num_elem = elementBlockGatherVisitor.totalNumberOfEntries();

  // Not doing sets and maps at the moment...
  meshParams.num_node_sets = 0;
  meshParams.num_edge_sets = 0;
  meshParams.num_face_sets = 0;
  meshParams.num_side_sets = 0;
  meshParams.num_elem_sets = 0;
  meshParams.num_node_maps = 0;
  meshParams.num_edge_maps = 0;
  meshParams.num_face_maps = 0;
  meshParams.num_elem_maps = 0;

  EXODUS_CALL( ex_put_init_ext( file.id(), &meshParams ),
    "Unable to initialize database.\n" );

  // Write the block data.
  std::vector< int > variableTruthTable(
    meshParams.num_edge_blk * edgeBlockGatherVisitor.maxVariableId(), 0 );
  for ( std::size_t blockIndex = 0; blockIndex < meshParams.num_edge_blk; ++blockIndex ) {
    Block& block = *edgeBlockGatherVisitor.pointers()[ blockIndex ];
    AttributeVariableVisitor visitor;
    block.traverse( visitor );
    EXODUS_CALL(
      ex_put_block(
        file.id(),
        EX_EDGE_BLOCK,
        block.id(),
        xdmGrid::exodusShapeString( block.topology()->cellType( 0 ) ).c_str(),
        (int)block.numberOfEntries(),
        (int)block.topology()->cellType( 0 ).nodesPerCell(),
        0, // edges per entry
        0, // faces per entry
        (int)visitor.attributes().size() ),
      "Unable to write edge block parameters." );

    std::vector< int > connections;
    for ( std::size_t entry = 0; entry < block.numberOfEntries(); ++entry ) {
      xdmGrid::ConstCellConnectivity entryNodes = block.topology()->cellConnections( entry );
      for ( std::size_t node = 0; node < entryNodes.size(); ++node ) {
        connections.push_back( (int)entryNodes[ node ] + 1 );
      }
    }
    EXODUS_CALL( ex_put_conn( file.id(), EX_EDGE_BLOCK, block.id(), &connections[0], 0, 0 ),
      "Unable to write edge block connectivity." );
    EXODUS_CALL( ex_put_name( file.id(), EX_EDGE_BLOCK, block.id(), block.name().c_str() ),
      "Unable to write edge block name." );

    // Write the Exodus attributes.
    std::vector< ExodusString > attributeNames;
    for ( std::size_t att = 0; att < visitor.attributes().size(); ++att ) {
      // Skip anything that isn't a scalar for now.
      if ( visitor.attributes()[ att ]->dataType() != xdmGrid::Attribute::kScalar ) {
        continue;
      }

      attributeNames.push_back( visitor.attributes()[ att ]->name() );
      EXODUS_CALL(
        ex_put_one_attr(
          file.id(),
          EX_EDGE_BLOCK,
          block.id(),
          (int)att,
          (void*)visitor.attributes()[ att ]->dataItem()->typedArray< double >()->begin() ),
        "Unable to write edge block attribute values." );

    }
    if ( visitor.attributes().size() > 0 ) {
      char* attributeNamesCharArray[ attributeNames.size() ];
      vectorToCharStarArray( attributeNames, attributeNamesCharArray );
      EXODUS_CALL(
        ex_put_attr_names( file.id(), EX_EDGE_BLOCK, block.id(), attributeNamesCharArray ),
        "Unable to write edge block attribute names." );
    }

    // Construct the truth table (this is not strictly necessary, but helps with efficiency).
    for ( std::size_t var = 0; var < visitor.variables().size(); ++var ) {
      variableTruthTable[ blockIndex * edgeBlockGatherVisitor.maxVariableId() +
        ( visitor.variables()[ var ]->id() - 1 ) ] = 1;
    }
  }

  // Write the Exodus variables.
  if ( edgeBlockGatherVisitor.maxVariableId() > 0 ) {
    EXODUS_CALL(
      ex_put_variable_param( file.id(), EX_EDGE_BLOCK, edgeBlockGatherVisitor.maxVariableId() ),
      "Unable to write edge result variable parameters." );
  }
  for ( int var = 1; var <= edgeBlockGatherVisitor.maxVariableId(); ++var ) {
    EXODUS_CALL(
      ex_put_variable_name(
        file.id(),
        EX_EDGE_BLOCK,
        var,
        edgeBlockGatherVisitor.names()[ var ].c_str() ),
      "Unable to write variable name." );
  }
  xdm::RefPtr< const xdmGrid::Time > time;
  for ( std::size_t blockIndex = 0; blockIndex < meshParams.num_edge_blk; ++blockIndex ) {
    Block& block = *edgeBlockGatherVisitor.pointers()[ blockIndex ];
    AttributeVariableVisitor visitor;
    block.traverse( visitor );
    for ( std::size_t var = 0; var < visitor.variables().size(); ++var ) {
      EXODUS_CALL(
        ex_put_var(
          file.id(),
          (int)( timeStep + 1 ),
          EX_EDGE_BLOCK,
          (int)( var + 1 ),
          block.id(),
          block.numberOfEntries(),
          (void*)visitor.variables()[ var ]->dataItem()->typedArray< double >()->begin() ),
        "Unable to write edge block variable." );
    }

    // Grab a time if one is available.
    if ( block.time().valid() ) {
      time = block.time();
    }
  }

  // Write the time step info.
  if ( time.valid() ) {
    double timeVal = time->value();
    EXODUS_CALL( ex_put_time( file.id(), (int)(timeStep + 1 ), (void*)&timeVal ), "Unable to write time value." );
  }
}

bool ExodusWriter::update(
  xdm::RefPtr< xdm::Item > item,
  const xdm::FileSystemPath& path,
  std::size_t timeStep ) {

  // TODO: implement
}

XDM_EXODUS_NAMESPACE_END
