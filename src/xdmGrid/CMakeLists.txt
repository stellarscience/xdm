project( xdmGrid )

set( ${PROJECT_NAME}_HEADERS 
    Domain.hpp
)

set( ${PROJECT_NAME}_SOURCES
    Domain.cpp
)

add_library( ${PROJECT_NAME} STATIC 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

add_subdirectory( test )

