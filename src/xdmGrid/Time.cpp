#include <xdmGrid/Time.hpp>

XDM_GRID_NAMESPACE_BEGIN

Time::Time( double value ) :
  mValue( value ) {
}

Time::~Time() {
}

void Time::setValue( double value ) {
  mValue = value;
}

double Time::value() const {
  return mValue;
}

void Time::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::Item::writeMetadata( xml );
  xml.setTag( "Time" );
  xml.setAttribute( "Value", mValue );
}

XDM_GRID_NAMESPACE_END

