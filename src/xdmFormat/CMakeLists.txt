project( xdmFormat )

set( ${PROJECT_NAME}_HEADERS 
    NamespaceMacro.hpp
    TemporalCollection.hpp
    TimeSeries.hpp
    VirtualDataset.hpp
    XdmfHelpers.hpp
)

set( ${PROJECT_NAME}_SOURCES
    TemporalCollection.cpp
    VirtualDataset.cpp
    XdmfHelpers.cpp
)

add_library( ${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
    xdm
    xdmGrid
    xdmHdf
)

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

