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

  double* nodeX() { return mNodeX; }
  double* nodeY() { return mNodeY; }
  double* nodeZ() { return mNodeZ; }

  std::size_t* connectivityArray() { return mConnectivity; }

  std::size_t numberOfNodes() { return 8; }

  std::size_t numberOfCells() { return 5; }

  CubeOfTets() {
    mNodeX[0] = 0.;
    mNodeX[1] = 1.;
    mNodeX[2] = 1.;
    mNodeX[3] = 0.;
    mNodeX[4] = 0.;
    mNodeX[5] = 1.;
    mNodeX[6] = 1.;
    mNodeX[7] = 0.;

    mNodeY[0] = 0.;
    mNodeY[1] = 0.;
    mNodeY[2] = 1.;
    mNodeY[3] = 1.;
    mNodeY[4] = 0.;
    mNodeY[5] = 0.;
    mNodeY[6] = 1.;
    mNodeY[7] = 1.;

    mNodeZ[0] = 0.;
    mNodeZ[1] = 0.;
    mNodeZ[2] = 0.;
    mNodeZ[3] = 0.;
    mNodeZ[4] = 1.;
    mNodeZ[5] = 1.;
    mNodeZ[6] = 1.;
    mNodeZ[7] = 1.;

    // Nodes go xyzxyzxyz...
    for ( int i = 0; i < 8; ++i ) {
      mNodes[i*3+0] = mNodeX[i];
      mNodes[i*3+1] = mNodeY[i];
      mNodes[i*3+2] = mNodeZ[i];
    }

    // Tetrahedra have 4 nodes each. The array goes cell0node0, cell0node1, ...cell5node3. Ordering
    // is ExodusII style (right hand rule on one face points thumb toward 4th node).
    mConnectivity[ 0] = 0; // tet 0
    mConnectivity[ 1] = 2;
    mConnectivity[ 2] = 5;
    mConnectivity[ 3] = 7;
    mConnectivity[ 4] = 2; // tet 1
    mConnectivity[ 5] = 5;
    mConnectivity[ 6] = 6;
    mConnectivity[ 7] = 7;
    mConnectivity[ 8] = 0; // tet 2
    mConnectivity[ 9] = 1;
    mConnectivity[10] = 2;
    mConnectivity[11] = 5;
    mConnectivity[12] = 0; // tet 3
    mConnectivity[13] = 5;
    mConnectivity[14] = 7;
    mConnectivity[15] = 4;
    mConnectivity[16] = 0; // tet 4
    mConnectivity[17] = 2;
    mConnectivity[18] = 3;
    mConnectivity[19] = 7;
  }

private:
  double mNodes[8*3];
  double mNodeX[8];
  double mNodeY[8];
  double mNodeZ[8];
  std::size_t mConnectivity[5*4];
};
