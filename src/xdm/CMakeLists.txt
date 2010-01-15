project( xdm )

find_package( HDF5 )

set( ${PROJECT_NAME}_HEADERS 
    Array.hpp
    Dataset.hpp
	Grid.hpp
	NamespaceMacro.hpp
    ReferencedObject.hpp
    RefPtr.hpp
    Slab.hpp
)
set( ${PROJECT_NAME}_SOURCES 
    Array.cpp
    Dataset.cpp
	Grid.cpp
    ReferencedObject.cpp
)

if( HDF5_FOUND )
    list( APPEND ${PROJECT_NAME}_HEADERS HdfDataset.hpp )
    list( APPEND ${PROJECT_NAME}_SOURCES HdfDataset.cpp )
endif()

include_directories(
    ${HDF5_INCLUDE_PATH}
)

add_library( ${PROJECT_NAME} 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME} 
    ${HDF5_LIBRARIES}
)

add_subdirectory( test )

