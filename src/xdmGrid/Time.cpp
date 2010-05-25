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
#include <xdmGrid/Time.hpp>

namespace xdmGrid {

Time::Time( double value ) :
  mValues( 1, value ) {
}

Time::~Time() {
}

void Time::setValue( double value ) {
  mValues.clear();
  mValues.push_back( value );
}

double Time::value() const {
  return mValues[0];
}

void Time::setValues( const std::vector< double >& values ) {
  mValues = values;
}

const std::vector< double >& Time::values() const {
  return mValues;
}

double Time::value( std::size_t atIndex ) const {
  return mValues[ atIndex ];
}

void Time::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::Item::writeMetadata( xml );
  xml.setTag( "Time" );
  xml.setAttribute( "Value", mValues[0] );
}

} // namespace xdmGrid

