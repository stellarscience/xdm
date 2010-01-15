//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmFormat_VirtualDataset_hpp
#define xdmFormat_VirtualDataset_hpp

#include <xdmFormat/TimeSeries.hpp>

#include <string>

#include <xdmFormat/NamespaceMacro.hpp>

XDM_FORMAT_NAMESPACE_BEGIN

/// Time series output target that writes one complete file per timestep. The
/// name VirtualDataset is derived from what the VisIt visualization application
/// calls such an output.  The use of this TimeSeries output target is a good
/// way to easily view the data in VisIt.
class VirtualDataset : public TimeSeries {
public:
  /// Construct a virtual dataset given a base name for all output files.  The
  /// output for a given time step will then take the form 
  /// metadataBaseName.<time_step>.xmf.
  VirtualDataset( const std::string& metadataBaseName );
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

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_VirtualDatasetTimeSeries_hpp

