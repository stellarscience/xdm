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
#ifndef xdmf_TemporalCollection_hpp
#define xdmf_TemporalCollection_hpp

#include <xdmf/TimeSeries.hpp>

#include <xdm/XmlOutputStream.hpp>

#include <fstream>
#include <string>



namespace xdmf {

/// Time series output that writes all grids as a temporal collection within a
/// single XDMF file.
class TemporalCollection : public TimeSeries {
public:
  /// Construct a temporal collection with  
  TemporalCollection( 
    const std::string& metadataFile,
    xdm::Dataset::InitializeMode mode );
  virtual ~TemporalCollection();

  virtual void open();
  virtual void updateGrid( xdm::RefPtr< xdmGrid::Grid > grid, std::size_t step );
  virtual void writeGridMetadata( xdm::RefPtr< xdmGrid::Grid > grid );
  virtual void writeGridData( xdm::RefPtr< xdmGrid::Grid > grid );
  virtual void close();

private:
  std::string mFilename;
  std::fstream mFileStream;
  xdm::XmlOutputStream mXmlStream;
};

} // namespace xdmf

#endif // xdmf_TemporalCollectionTimeSeries_hpp

