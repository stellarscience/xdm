#
# Exodus II plugin for XDM
#
project( xdmExodus )

set( ${PROJECT_NAME}_HEADERS
    ExodusIO.hpp
    NamespaceMacro.hpp
)

set( ${PROJECT_NAME}_SOURCES
    ExodusIO.cpp
)

add_library( ${PROJECT_NAME} 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
  exoIIv2c )

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

