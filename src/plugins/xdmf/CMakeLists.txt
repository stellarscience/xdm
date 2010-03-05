project( xdmXdmfPlugin )

find_package( LibXml2 REQUIRED )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. )

set( ${PROJECT_NAME}_HEADERS
  LinearTopologyData.hpp
  NamespaceMacro.hpp
  TemporalCollection.hpp
  TimeSeries.hpp
  VirtualDataset.hpp
  XdmfHelpers.hpp
  XmfReader.hpp
)
set( ${PROJECT_NAME}_SOURCES 
  LinearTopologyData.cpp
  TemporalCollection.cpp
  VirtualDataset.cpp
  XdmfHelpers.cpp
  XmfReader.cpp
)

add_definitions( ${LIBXML2_DEFINITIONS} )

include_directories( ${LIBXML2_INCLUDE_DIR} )

add_library( ${PROJECT_NAME} 
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
  xdm
  xdmFormat
  xdmGrid
  xdmHdf
  ${LIBXML2_LIBRARIES}
)

if( BUILD_TESTING )
  add_subdirectory( test )
endif()
