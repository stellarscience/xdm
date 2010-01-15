project( xdmHdf )

find_package( HDF5 REQUIRED )

set( ${PROJECT_NAME}_HEADERS 
    DatasetIdentifier.hpp
    FileIdentifier.hpp
    FileIdentifierRegistry.hpp
    GroupIdentifier.hpp
    HdfDataset.hpp
    ResourceIdentifier.hpp
    SelectionVisitor.hpp
)

set( ${PROJECT_NAME}_SOURCES 
    DatasetIdentifier.cpp
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

add_subdirectory( test )

