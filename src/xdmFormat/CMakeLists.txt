project( xdmFormat )

set( ${PROJECT_NAME}_HEADERS 
    Namespace.hpp
    Reader.hpp
    ReadResult.hpp
    Writer.hpp
)

set( ${PROJECT_NAME}_SOURCES
    Reader.cpp
    ReadResult.cpp
    Writer.cpp
)

add_library( ${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
    xdm
)

