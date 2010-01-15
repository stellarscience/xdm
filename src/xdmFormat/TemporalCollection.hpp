//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmFormat_TemporalCollection_hpp
#define xdmFormat_TemporalCollection_hpp

#include <xdmFormat/TimeSeries.hpp>

#include <xdm/XmlOutputStream.hpp>

#include <fstream>
#include <string>

#include <xdmFormat/NamespaceMacro.hpp>

XDM_FORMAT_NAMESPACE_BEGIN

/// Time series output that writes all grids as a temporal collection within a
/// single XDMF file.
class TemporalCollection : public TimeSeries {
public:
  /// Construct a temporal collection with  
  TemporalCollection( const std::string& metadataFile );
  virtual ~TemporalCollection();

  virtual void open();
  virtual void updateGrid( xdm::RefPtr< xdmGrid::Grid > grid );
  virtual void writeGridMetadata( xdm::RefPtr< xdmGrid::Grid > grid );
  virtual void writeGridData( xdm::RefPtr< xdmGrid::Grid > grid );
  virtual void close();

private:
  std::string mFilename;
  std::fstream mFileStream;
  xdm::XmlOutputStream mXmlStream;
};

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_TemporalCollectionTimeSeries_hpp

