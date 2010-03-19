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
#ifndef xdmExodus_ExodusReader_hpp
#define xdmExodus_ExodusReader_hpp

#include <xdmFormat/Reader.hpp>

#include <xdmExodus/NamespaceMacro.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

/// Class for reading an ExodusII file. Uses the ExodusII library functions to read
/// an unstructured grid from an ExodusII file complete with nodes and element blocks
/// for now.
class ExodusReader : public xdmFormat::Reader {
public:
  ExodusReader();
  virtual ~ExodusReader();

  /// Read a complete ExodusII file.
  /// ExodusII files are structured around a single invariant geometry with invariant topologies.
  /// If the geometry or topology must change, then additional ExodusII files are necessary. In
  /// addition, there are a few nomenclature clashes with XDM. For ExodusII, all "attributes" are
  /// static values that may be assigned to different portions of the mesh. ExodusII "variables"
  /// are time varying values. Since XDM does not distinguish between the two, everything becomes
  /// an xdmGrid::Attribute. An example of the conversion of a thermal problem on a fixed mesh
  /// with results for several time steps is shown below.
  /// Domain
  ///   CollectionGrid (1 Grid per time step)
  ///     Time (whole series)
  ///     CollectionGrid (time step 1)
  ///       Time (for just this time step)
  ///       UniformGrid
  ///         Name
  ///         Geometry (points to the only one)
  ///         Topology (one edge/face/element block)
  ///         Attribute (thermal conductivity for this block for all time)
  ///         Attribute (variable 1 for each element)
  ///         Attribute (variable 2 for each element)
  ///       UniformGrid
  ///         Name
  ///         Geometry (points to only one)
  ///         Topology (one block)
  ///         Attribute (variable 1 for each element)
  ///     CollectionGrid (time step 2)
  ///       Identical to one above except attributes (just the variables) have changed values. This
  ///       grid contains pointers to the same topologies as above (the actual data values are only
  ///       stored once).
  ///     CollectionGrid (time step 3)
  ///       Identical to one above except attributes (just the variables) have changed values
  ///          ...
  /// @return an xdmGrid::Domain
  virtual xdm::RefPtr< xdm::Item > readItem( const std::string& filename );

};

XDM_EXODUS_NAMESPACE_END

#endif // xdmExodus_ExodusReader_hpp

