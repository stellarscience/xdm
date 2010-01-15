#ifndef xdmFormat_TimeSeries_hpp
#define xdmFormat_TimeSeries_hpp

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <xdmFormat/NamespaceMacro.hpp>

namespace xdmGrid {
  class Grid;
}

XDM_FORMAT_NAMESPACE_BEGIN

/// Interface that inheritors should reimplement in order to write time
/// series grid information.
class TimeSeries : public xdm::ReferencedObject {
public:
  TimeSeries() {}
  virtual ~TimeSeries() {}

  /// Open a new time series.  Opens a new time series and prepares it for
  /// accepting timestep grids.
  virtual void open() = 0;

  /// Update the grid for a new timestep.
  virtual void updateGrid( xdm::RefPtr< xdmGrid::Grid > grid ) = 0;

  /// Write the metadata for a grid.
  virtual void writeGridMetadata( xdm::RefPtr< xdmGrid::Grid > grid ) = 0;

  /// Write the large datasets for the grid.
  virtual void writeGridData( xdm::RefPtr< xdmGrid::Grid > grid ) = 0;

  /// Close the time series.
  virtual void close() = 0;
};

/// Convenience function to perform all steps of writing a TimeSeries grid.
inline void writeTimestepGrid( 
  xdm::RefPtr< TimeSeries > timeSeries, 
  xdm::RefPtr< xdmGrid::Grid > grid ) {

  timeSeries->updateGrid( grid );
  timeSeries->writeGridMetadata( grid );
  timeSeries->writeGridData( grid );

}
  

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_TimeSeries_hpp

