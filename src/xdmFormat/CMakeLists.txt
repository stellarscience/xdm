project( xdmFormat )

find_package( LibXml2 REQUIRED )

add_definitions( ${LIBXML2_DEFINITIONS} )

include_directories( ${LIBXML2_INCLUDE_DIR} )

set( ${PROJECT_NAME}_HEADERS 
    NamespaceMacro.hpp
    Reader.hpp
    TemporalCollection.hpp
    TimeSeries.hpp
    VirtualDataset.hpp
    XdmfHelpers.hpp
    XmfReader.hpp
)

set( ${PROJECT_NAME}_SOURCES
    Reader.cpp
    TemporalCollection.cpp
    VirtualDataset.cpp
    XdmfHelpers.cpp
    XmfReader.cpp
)

add_library( ${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
    xdm
    xdmGrid
    xdmHdf
    ${LIBXML2_LIBRARIES}
)

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

