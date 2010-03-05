project( xdmFormat )

set( ${PROJECT_NAME}_HEADERS 
    NamespaceMacro.hpp
    Reader.hpp
)

set( ${PROJECT_NAME}_SOURCES
    Reader.cpp
)

add_library( ${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
    xdm
)

