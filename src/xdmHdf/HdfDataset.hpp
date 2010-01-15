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
#ifndef xdmHdf_HdfDataset_hpp
#define xdmHdf_HdfDataset_hpp

#include <xdm/Dataset.hpp>

#include <memory>
#include <string>
#include <vector>

#include <xdmHdf/NamespaceMacro.hpp>

XDM_HDF_NAMESPACE_BEGIN

/// Path of groups identifying a location in the HDF file.
typedef std::vector< std::string > GroupPath;

class HdfDataset : public xdm::Dataset {
public:
  /// Default constructor does not associate with a file.
  HdfDataset();
  /// Constructor takes file, group, and dataset names.
  HdfDataset( 
    const std::string& file, 
    const GroupPath& groupPath,
    const std::string& dataset );
  virtual ~HdfDataset();

  /// Set the file name.
  void setFile( const std::string& file );
  /// Get the file name.
  const std::string& file() const;

  /// Set the group path in the HDF file.
  void setGroupPath( const GroupPath& group );
  /// Get the group name.
  const GroupPath& groupPath() const;

  /// Set the dataset name.
  void setDataset( const std::string& dataset );
  /// Get the dataset name.
  const std::string& dataset() const;

  //-- Dataset Implementations --//
  virtual const char* format() { return "HDF"; }
  virtual void writeTextContent( xdm::XmlTextContent& text );
  virtual void initializeImplementation(
    xdm::primitiveType::Value type,
    const xdm::DataShape<>& shape );
  virtual void serializeImplementation( 
    const xdm::StructuredArray* data,
    const xdm::DataSelectionMap& selectionMap );
  virtual void finalizeImplementation();

private:
  struct Private;
  std::auto_ptr< Private > imp;
};

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_HdfDataset_hpp

