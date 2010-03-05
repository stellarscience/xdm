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
#ifndef xdmf_VirtualDataset_hpp
#define xdmf_VirtualDataset_hpp

#include <xdmf/TimeSeries.hpp>

#include <string>

#include <xdmf/NamespaceMacro.hpp>

XDMF_NAMESPACE_BEGIN

/// Time series output target that writes one complete file per timestep. The
/// name VirtualDataset is derived from what the VisIt visualization application
/// calls such an output.  The use of this TimeSeries output target is a good
/// way to easily view the data in VisIt.
class VirtualDataset : public TimeSeries {
public:
  /// Construct a virtual dataset given a base name for all output files.  The
  /// output for a given time step will then take the form 
  /// metadataBaseName.<time_step>.xmf.
  VirtualDataset( 
    const std::string& metadataBaseName,
    xdm::Dataset::InitializeMode mode );
  virtual ~VirtualDataset();

  virtual void open();
  virtual void updateGrid( xdm::RefPtr< xdmGrid::Grid > grid );
  virtual void writeGridMetadata( xdm::RefPtr< xdmGrid::Grid > grid );
  virtual void writeGridData( xdm::RefPtr< xdmGrid::Grid > grid );
  virtual void close();

private:
  std::string mBaseName;
  unsigned int mTimeStep;
};

XDMF_NAMESPACE_END

#endif // xdmf_VirtualDatasetTimeSeries_hpp

