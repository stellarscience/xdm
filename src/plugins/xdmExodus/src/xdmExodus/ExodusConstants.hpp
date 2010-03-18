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
#include <exodusII.h>

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

/// Const types and some functions that are useful when working with ExodusII files.

#define EXODUS_CALL( functionCall, errorString ) \
  if ( (#functionCall) < 0 ) { \
    throw std::runtime_error( (#errorString) ); \
  }

XDM_EXODUS_NAMESPACE_BEGIN

/// String type for working with Exodus char*.
struct ExodusString {
  char raw[ MAX_STR_LENGTH + 1 ];

  char* ptr() { return raw; }

  std::string string() { return std::string( raw ); }
};

/// Get a char** that points to the individual strings in a std::vector< ExodusString >.
void vectorToCharStarArray( std::vector< ExodusString >& input, char* output[] ) {
  std::transform( input.begin(), input.end(), output,
    std::mem_fun_ref( &ExodusString::ptr ) );
}

/// Do two conversions: int -> size_t and also move the base to zero. Exodus numbering starts at
/// 1, so this subtracts 1 from all values to give zero-based indexing.
std::vector< std::size_t > convertToZeroBase( const std::vector< int >& vecWithExodusOrdering ) {
  std::vector< std::size_t > result;
  std::transform( vecWithExodusOrdering.begin(), vecWithExodusOrdering.end(), result.begin(),
    std::bind2nd( std::minus< std::size_t >(), 1 ) );
  return result;
}

const std::size_t kNumberOfObjectTypes = 12;

bool objectIsBlock( std::size_t i ) { return i >= 0 && i < 3; }
bool objectIsSet( std::size_t i ) { return i > 2 && i < 8; }
bool objectIsMap( std::size_t i ) { return i >= 8 && i < kNumberOfObjectTypes; }

/// These are the flags that Exodus uses to signify edge blocks, node sets, etc.
const int kObjectTypes[ kNumberOfObjectTypes ] = {
  EX_EDGE_BLOCK,
  EX_FACE_BLOCK,
  EX_ELEM_BLOCK,
  EX_NODE_SET,
  EX_EDGE_SET,
  EX_FACE_SET,
  EX_SIDE_SET,
  EX_ELEM_SET,
  EX_NODE_MAP,
  EX_EDGE_MAP,
  EX_FACE_MAP,
  EX_ELEM_MAP
};

/// Pass these flags to ex_inquire to find the number of objects of the type given by
/// kObjectTypes. For example, ex_inquire( file, kInquireObjectSizes[ 2 ], int*, 0, 0 )
/// returns the number of element blocks in the int* (because kObjectTypes[ 2 ] is for
/// element blocks).
const int kInquireObjectSizes[ kNumberOfObjectTypes ] = {
  EX_INQ_EDGE_BLK,
  EX_INQ_FACE_BLK,
  EX_INQ_ELEM_BLK,
  EX_INQ_NODE_SETS,
  EX_INQ_EDGE_SETS,
  EX_INQ_FACE_SETS,
  EX_INQ_SIDE_SETS,
  EX_INQ_ELEM_SETS,
  EX_INQ_NODE_MAP,
  EX_INQ_EDGE_MAP,
  EX_INQ_FACE_MAP,
  EX_INQ_ELEM_MAP
};

const ExodusString kObjectTypenames[ kNumberOfObjectTypes ] = {
  { "   Edge block" },
  { "   Face block" },
  { "Element block" },
  { "    Node set" },
  { "    Edge set" },
  { "    Face set" },
  { "    Side set" },
  { " Element set" },
  { "    Node map" },
  { "    Edge map" },
  { "    Face map" },
  { " Element map" }
};

const char* kObjectTypeChar[ kNumberOfObjectTypes ] = {
  "L",
  "F",
  "E",
  "M",
  "D",
  "A",
  "S",
  "T",
  0, /* maps have no result variables */
  0,
  0,
  0,
};

XDM_EXODUS_NAMESPACE_END

