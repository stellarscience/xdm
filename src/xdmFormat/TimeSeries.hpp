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

  /// Write a grid to the time series for the specified timestep.  Writes the
  /// grid data for the given timestep.
  virtual void writeTimestepGrid( xdm::RefPtr< xdmGrid::Grid > grid ) = 0;

  /// Close the time series.
  virtual void close() = 0;
};

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_TimeSeries_hpp

