project( xdmFormat )

set( ${PROJECT_NAME}_HEADERS 
    XdmfHelpers.hpp
)

set( ${PROJECT_NAME}_SOURCES
    NamespaceMacro.hpp
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

