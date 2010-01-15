project( xdmHdf )

set( HDF5_USE_STATIC_LIBRARIES TRUE CACHE BOOL
    "Favor static HDF5 libraries" )
mark_as_advanced( HDF5_USE_STATIC_LIBRARIES )
find_package( HDF5 REQUIRED )

set( ${PROJECT_NAME}_HEADERS 
    AttachHdfDatasetOperation.hpp
    DatasetIdentifier.hpp
    DataspaceIdentifier.hpp
    FileIdentifier.hpp
    FileIdentifierRegistry.hpp
    GroupIdentifier.hpp
    HdfDataset.hpp
    ResourceIdentifier.hpp
    SelectionVisitor.hpp
)

set( ${PROJECT_NAME}_SOURCES 
    AttachHdfDatasetOperation.cpp
    DatasetIdentifier.cpp
    DataspaceIdentifier.cpp
    FileIdentifier.cpp
    FileIdentifierRegistry.cpp
    GroupIdentifier.cpp
    HdfDataset.cpp
    SelectionVisitor.cpp
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

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

