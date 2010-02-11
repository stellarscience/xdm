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

// Code Review Matter (open): Doxygen comment.
// Did you consider providing documentation for this class so that others may
// gain understanding of the class interface and caveats of use?
// Will Dicharry 2010-01-19
class HdfDataset : public xdm::Dataset {
public:
  /// Default constructor does not associate with a file.
  HdfDataset();

  // Code Review Matter (open): argument documentation
  // Did you consider documenting the nature of file, groupPath,
  // and dataset? i.e., What is a GroupPath? Could you provide an
  // example of what one "looks" like? Are there helper methods
  // that construct them?
  // For what is a dataset name used? Are there limitations to its
  // content or structure (spaces, punctuation, etc.)?
  // -- K. R. Walker on 2010-01-19

  // Code Review Matter (open): file path
  // Did you consider using a class to represent a file path?
  // -- K. R. Walker on 2010-01-19

  // Code Review Matter (open): file path
  // Did you consider using a class to represent a dataset name
  // (assuming it has constraints or invariants)?
  // -- K. R. Walker on 2010-01-19

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

  /// Choose to use chunked IO or not. Used with setChunkSize, chunked IO can
  /// be used to minimize file IO operations. If Chunked IO is used, but the
  /// Chunk size is not set, then at Dataset initialization, the dimensions
  /// of the entire dataset will be used for the chunk size.
  /// @param value Whether or not to use chunked IO.
  void setUseChunkedIo( bool value );
  /// Set the chunk size for the dataset. Intelligently chosen chunk sizes can
  /// minimize file IO.
  /// @param dimensions Chunk dimensions for the dataset.
  /// @post Initialize must be called with a space of the same rank as the
  /// chunk size.
  void setChunkSize( const xdm::DataShape<>& dimensions );

  /// Turn compression on or off for the dataset. Enabling compression implies
  /// that chunked IO must be used. If the HDF5 library was built without the
  /// compression filter, this method has no effect.
  /// @param value Whether or not to compress the data.
  /// @post Compression and Chunked IO are enabled.
  void setUseCompression( bool value );
  /// Set the compression level for the dataset. Valid values are 0-9. Lower
  /// numbers mean less compression but faster writes, higher numbers yield
  /// more compression but slower write time. Following GZip, the default
  /// compression level is 6.
  /// @param level Integer between 0 and 9 to determine compression level.
  void setCompressionLevel( size_t level );

  //-- Dataset Implementations --//
  virtual const char* format() { return "HDF"; }

  // Code Review Matter (open): Namespace Macros
  // There are macros that optionally create namespaces. If the namespaces
  // are not used, then the following member functions should fail to
  // compile because they explicitly use those namespaces. Did you consider
  // creating a namespace macro instances like these? e.g.,
  // #define XDM_NAMESPACE xdm
  // XDM_NAMESPACE::primitiveType::Value
  // -- K. R. Walker on 2010-01-19

  virtual void writeTextContent( xdm::XmlTextContent& text );
  
  virtual void initializeImplementation(
    xdm::primitiveType::Value type,
    const xdm::DataShape<>& shape,
    const xdm::Dataset::InitializeMode& mode );

  virtual void serializeImplementation( 
    const xdm::StructuredArray* data,
    const xdm::DataSelectionMap& selectionMap );

  virtual void deserializeImplementation(
    xdm::StructuredArray* data,
    const xdm::DataSelectionMap& selectionMap );

  virtual void finalizeImplementation();

private:

  // Code Review Matter (open): imp vs mImp
  // The Stellar software standards doesn't call this out specifically,
  // but did you consider using the defacto standard mImp over imp.
  struct Private;
  std::auto_ptr< Private > imp;
};

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_HdfDataset_hpp

