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
#ifndef xdmf_TimeSeries_hpp
#define xdmf_TimeSeries_hpp

#include <xdm/Dataset.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <xdmGrid/Grid.hpp>



namespace xdmf {

/// Interface that inheritors should reimplement in order to write time
/// series grid information.
class TimeSeries : public xdm::ReferencedObject {
public:
  TimeSeries( xdm::Dataset::InitializeMode mode ) : mMode( mode ) {}
  virtual ~TimeSeries() {}

  /// Open a new time series.  Opens a new time series and prepares it for
  /// accepting timestep grids.
  virtual void open() = 0;

  /// Update the grid for a new timestep.
  virtual void updateGrid( xdm::RefPtr< xdmGrid::Grid > grid, std::size_t step ) = 0;

  /// Write the metadata for a grid.
  virtual void writeGridMetadata( xdm::RefPtr< xdmGrid::Grid > grid ) = 0;

  /// Write the large datasets for the grid.
  virtual void writeGridData( xdm::RefPtr< xdmGrid::Grid > grid ) = 0;

  /// Close the time series.
  virtual void close() = 0;

  xdm::Dataset::InitializeMode mode() const { return mMode; }
  void setMode( xdm::Dataset::InitializeMode mode ) { mMode = mode; }

private:
  xdm::Dataset::InitializeMode mMode;
};

/// Convenience function to perform all steps of writing a TimeSeries grid.
inline void writeTimestepGrid( 
  xdm::RefPtr< TimeSeries > timeSeries, 
  xdm::RefPtr< xdmGrid::Grid > grid,
  std::size_t step ) {

  timeSeries->updateGrid( grid, step );
  timeSeries->writeGridMetadata( grid );
  timeSeries->writeGridData( grid );

}
  

} // namespace xdmf

#endif // xdmf_TimeSeries_hpp

