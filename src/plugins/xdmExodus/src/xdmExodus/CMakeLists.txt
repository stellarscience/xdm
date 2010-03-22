#
# Exodus II plugin for XDM
#
project( xdmExodus )

set( ${PROJECT_NAME}_HEADERS
    ExodusConstants.hpp
    ExodusReader.hpp
    NamespaceMacro.hpp
)

set( ${PROJECT_NAME}_SOURCES
    ExodusReader.cpp
)

add_library( ${PROJECT_NAME} 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
  exoIIv2c
  xdm
  xdmFormat
  xdmGrid )

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

