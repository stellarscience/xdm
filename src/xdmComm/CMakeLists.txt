project( xdmComm )

find_package( MPI REQUIRED )

set( ${PROJECT_NAME}_HEADERS 
)

set( ${PROJECT_NAME}_SOURCES 
)

add_definitions( 
    ${MPI_COMPILE_FLAGS}
)

include_directories(
    ${MPI_INCLUDE_PATH}
)

add_library( ${PROJECT_NAME} STATIC
    ${PROJECT_NAME}_HEADERS
    ${PROJECT_NAME}_SOURCES
)

target_link_libraries( ${PROJECT_NAME}
    xdm
    ${MPI_LIBRARIES}
)

