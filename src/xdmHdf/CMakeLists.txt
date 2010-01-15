project( xdmHdf )

find_package( HDF5 REQUIRED )

set( ${PROJECT_NAME}_HEADERS 
    HdfDataset.hpp
)

set( ${PROJECT_NAME}_SOURCES 
    HdfDataset.cpp
)

include_directories(
    ${HDF5_INCLUDE_DIRS}
)

add_library( ${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
)

target_link_libraries( ${PROJECT_NAME}
    xdm
    ${HDF5_LIBRARIES}
)

add_subdirectory( test )

