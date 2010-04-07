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

#include <xdm/FileSystem.hpp>

#include <fstream>

#include <cstdlib>

XDM_NAMESPACE_BEGIN

FileSystemPath::FileSystemPath() :
  mPathString()
{
}

FileSystemPath::FileSystemPath( const std::string& pathString ) :
  mPathString( pathString )
{
}

FileSystemPath::FileSystemPath( const char * pathString ) :
  mPathString( pathString )
{
}

FileSystemPath::~FileSystemPath()
{
}

const std::string& FileSystemPath::pathString() const
{
  return mPathString;
}

void FileSystemPath::setPathString( const std::string& pathString )
{
  mPathString = pathString;
}

bool operator<( const FileSystemPath& lhs, const FileSystemPath& rhs ) {
  return ( lhs.pathString() < rhs.pathString() );
}

bool exists( const FileSystemPath& path )
{
  std::ifstream file( path.pathString().c_str() );
  return ( file.good() && file.is_open() );
}

size_t size( const FileSystemPath& path )
{
  std::ifstream file( path.pathString().c_str() );
  if ( !file.good() || file.eof() || !file.is_open() ) {
    return 0;
  }
  file.seekg( 0, std::ios_base::beg );
  std::ifstream::pos_type beginpos = file.tellg();
  file.seekg( 0, std::ios_base::end );
  return ( file.tellg() - beginpos );
}

bool remove( const FileSystemPath& path )
{
  if ( !exists( path ) ) {
    return false;
  }
  return ( ::remove( path.pathString().c_str() ) == 0 );
}

XDM_NAMESPACE_END

