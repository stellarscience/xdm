#
# Exodus II plugin for XDM
#
project( xdmExodus )

set( ${PROJECT_NAME}_HEADERS
    Blocks.hpp
    Helpers.hpp
    Maps.hpp
    Object.hpp
    Reader.hpp
    Sets.hpp
    Variable.hpp
    Writer.hpp
    NamespaceMacro.hpp
)

set( ${PROJECT_NAME}_SOURCES
    Blocks.cpp
    Object.cpp
    Reader.cpp
    Variable.cpp
    Writer.cpp
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

