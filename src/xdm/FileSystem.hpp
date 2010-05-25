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
#ifndef xdm_FileSystem_hpp
#define xdm_FileSystem_hpp

#include <string>



namespace xdm {

/// Representation of a file path on disk.
class FileSystemPath
{
public:
  FileSystemPath();
  explicit FileSystemPath( const std::string& pathString );
  explicit FileSystemPath( const char * pathString );
  ~FileSystemPath();

  const std::string& pathString() const;
  void setPathString( const std::string& path );

private:
  std::string mPathString;
};

/// Less than comparator for FileSystemPath objects.
bool operator<( const FileSystemPath& lhs, const FileSystemPath& rhs );

/// Determine if a file exists on disk.
bool exists( const FileSystemPath& path );

/// Determine the size of a file on disk.
/// @return The size of the file in bytes.
size_t size( const FileSystemPath& path );

/// Delete the file if it exists. If the file does not exist, then nothing is
/// done and the function returns false.
/// @return True if deleted, false otherwise.
bool remove( const FileSystemPath& path );

} // namespace xdm

#endif // xdm_FileSystem_hpp

