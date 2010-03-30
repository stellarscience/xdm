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
#ifndef xdmExodus_Reader_hpp
#define xdmExodus_Reader_hpp

#include <xdmFormat/Reader.hpp>

#include <xdmExodus/NamespaceMacro.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

/// Class for reading an ExodusII file. Uses the ExodusII library functions to read
/// an unstructured grid from an ExodusII file complete with nodes and element blocks
/// for now.
class ExodusReader {
public:
  ExodusReader();
  virtual ~ExodusReader();

  /// Read a complete ExodusII file.
  /// ExodusII files are structured around a single invariant geometry with invariant topologies.
  /// If the geometry or topology must change, then additional ExodusII files are necessary. In
  /// addition, there are a few nomenclature clashes with XDM. For ExodusII, all "attributes" are
  /// static values that may be assigned to different portions of the mesh. ExodusII "variables"
  /// are time varying values. Since XDM does not distinguish between the two, everything becomes
  /// an xdmGrid::Attribute.
  ///
  /// This reader creates a Domain with a single spatial CollectionGrid underneath that contains
  /// all of the Exodus blocks, sets, and maps. If there is dynamic information (variables) then
  /// readItem will set up these variables, but a call to update() is needed to read a time step.
  ///
  /// The example below shows a result that might be obtained when reading a thermal mesh.
  ///
  /// Domain
  ///   CollectionGrid (a spatial collection)
  ///     UniformGrid
  ///       Name
  ///       Geometry (points to the only one)
  ///       Topology (one edge/face/element block)
  ///       Attribute (thermal conductivity for this block for all time)
  ///       Attribute (variable 1 for each element)
  ///       Attribute (variable 2 for each element)
  ///     UniformGrid
  ///       Name
  ///       Geometry (points to only one)
  ///       Topology (one block)
  ///       Attribute (variable 1 for each element)
  ///          ...
  /// @return an xdmGrid::Domain
  virtual xdm::RefPtr< xdm::Item > readItem( const xdm::FileSystemPath& path );

  /// Read the data for a particular time step.
  /// @post A Time object is attached to all Grid items and all Attributes that have dynamic
  ///       data (Exodus variables) have updated values.
  /// @see xdmFormat::Reader
  virtual bool update(
    xdm::RefPtr< xdm::Item > item,
    const xdm::FileSystemPath& path,
    std::size_t timeStep = 0 );

  /// Get the number of time steps in the ExodusII file.
  std::size_t numberOfTimeSteps( const xdm::FileSystemPath& path ) const;

};

XDM_EXODUS_NAMESPACE_END

#endif // xdmExodus_Reader_hpp
