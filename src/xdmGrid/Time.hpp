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
#ifndef xdmGrid_Time_hpp
#define xdmGrid_Time_hpp

#include <xdm/DataItem.hpp>

#include <vector>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Time : public xdm::Item {
public:
  explicit Time( double value = 0.0 );
  virtual ~Time();

  /// Set the time value for this item.
  void setValue( double value );
  double value() const;

  /// Set a time series.
  void setValues( const std::vector< double >& values );
  const std::vector< double >& values() const;
  double value( std::size_t atIndex ) const;

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

private:
  std::vector< double > mValues;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Time_hpp

