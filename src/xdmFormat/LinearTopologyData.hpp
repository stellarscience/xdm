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
#ifndef xdmFormat_LinearTopologyData_hpp
#define xdmFormat_LinearTopologyData_hpp

#include <xdm/MemoryAdapter.hpp>
#include <xdm/RefPtr.hpp>

#include <xdmFormat/NamespaceMacro.hpp>

namespace xdm {
  class UniformDataItem;
  template< typename T > class VectorStructuredArray;
}

namespace xdmGrid {
  class Polyvertex;
}

XDM_FORMAT_NAMESPACE_BEGIN

/// MemoryAdapter that generates a list of unsigned integers up to the size of
/// the referenced Polyvertex Topology. This is useful if the XDMF output will
/// be used in VisIt, as it requires explicit specification of the connectivity
/// of Polyvertex topology.
///
/// This class uses 32-bit signed integers to hold the data in order to deal
/// with a ParaView bug that causes the system to crash if the data is held as
/// an unsigned integer.
class LinearTopologyData : public xdm::MemoryAdapter {
public:
  /// Construct with a topology to use as a reference for the list of numbers
  /// to generate.
  LinearTopologyData( xdm::RefPtr< xdmGrid::Polyvertex > topology );
  virtual ~LinearTopologyData();

  virtual xdm::RefPtr< xdm::StructuredArray > array();

protected:
  virtual void writeImplementation( xdm::Dataset* dataset );

private:
  xdm::RefPtr< xdmGrid::Polyvertex > mTopology;
  // See the class doxygen documentation for why this is 32-bit signed.
  xdm::RefPtr< xdm::VectorStructuredArray< int > > mArrayValues;
};

/// Convenience function to construct a UniformDataItem that will track the size
/// of the input Topology.
xdm::RefPtr< xdm::UniformDataItem >
createLinearTopologyUniformDataItem(
  xdm::RefPtr< xdmGrid::Polyvertex > topology );

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_LinearTopologyData_hpp
