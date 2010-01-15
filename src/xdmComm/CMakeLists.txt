project( xdmComm )

find_package( MPI REQUIRED )

set( ${PROJECT_NAME}_HEADERS 
    BinaryIosBase.hpp
    BinaryIStream.hpp
    BinaryIOStream.hpp
    BinaryOStream.hpp
    BinaryStreamBuffer.hpp
    MpiDatasetProxy.hpp
    NamespaceMacro.hpp
)

set( ${PROJECT_NAME}_SOURCES
    BinaryIStream.cpp
    BinaryIOStream.cpp
    BinaryOStream.cpp
    MpiDatasetProxy.cpp
)

add_definitions( 
    ${MPI_COMPILE_FLAGS}
)

include_directories(
    ${MPI_INCLUDE_PATH}
)

add_library( ${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
    xdm
    ${MPI_LIBRARIES}
)

if( UNIT_TEST_XDM )
    add_subdirectory( test )
endif()

