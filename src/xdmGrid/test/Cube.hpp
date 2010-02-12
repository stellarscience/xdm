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
#include <vector>

/// Test helper class that represents a cube. The vertices are the 8 corners of
/// the cube. The topology is tetrahedral.
class CubeOfTets {
public:
  double* nodeArray() { return mNodes; }

  std::size_t* connectivityArray() { return mConnectivity; }

  std::size_t numberOfNodes() { return 8; }

  std::size_t numberOfCells() { return 5; }

  CubeOfTets() {
    // Nodes go xyzxyzxyz...
    mNodes[0] = 0.; // node 0
    mNodes[1] = 0.;
    mNodes[2] = 0.;
    mNodes[3] = 1.; // node 1
    mNodes[4] = 0.;
    mNodes[5] = 0.;
    mNodes[6] = 1.; // node 2
    mNodes[7] = 1.;
    mNodes[8] = 0.;
    mNodes[9] = 0.; // node 3
    mNodes[10] = 1.;
    mNodes[11] = 0.;
    mNodes[12] = 0.; // node 4
    mNodes[13] = 0.;
    mNodes[14] = 1.;
    mNodes[15] = 1.; // node 5
    mNodes[16] = 0.;
    mNodes[17] = 1.;
    mNodes[18] = 1.; // node 6
    mNodes[19] = 1.;
    mNodes[20] = 1.;
    mNodes[21] = 0.; // node 7
    mNodes[22] = 1.;
    mNodes[23] = 1.;

    // Tetrahedra have 4 nodes each. The array goes cell0node0, cell0node1, ...cell5node3. Ordering
    // is ExodusII style (right hand rule on one face points thumb toward 4th node).
    mConnectivity[0] = 0; // tet 0
    mConnectivity[0] = 2;
    mConnectivity[0] = 5;
    mConnectivity[0] = 7;
    mConnectivity[0] = 2; // tet 1
    mConnectivity[0] = 5;
    mConnectivity[0] = 6;
    mConnectivity[0] = 7;
    mConnectivity[0] = 0; // tet 2
    mConnectivity[0] = 1;
    mConnectivity[0] = 2;
    mConnectivity[0] = 5;
    mConnectivity[0] = 0; // tet 3
    mConnectivity[0] = 5;
    mConnectivity[0] = 7;
    mConnectivity[0] = 4;
    mConnectivity[0] = 0; // tet 4
    mConnectivity[0] = 2;
    mConnectivity[0] = 3;
    mConnectivity[0] = 7;
  }

private:
  double mNodes[8*3];
  std::size_t mConnectivity[5*4];
};
