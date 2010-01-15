project( xdmf )

find_package( HDF5 )

set( ${PROJECT_NAME}_HEADERS 
    Array.hpp
    Dataset.hpp
	DataItem.hpp
	Domain.hpp
	Geometry.hpp
	Grid.hpp
	Item.hpp
	ItemVisitor.hpp
	NamespaceMacro.hpp
    Slab.hpp
	Topology.hpp
)
set( ${PROJECT_NAME}_SOURCES 
    Array.cpp
    Dataset.cpp
	DataItem.cpp
	Domain.cpp
	Geometry.cpp
	Grid.cpp
	Item.cpp
	ItemVisitor.cpp
	Topology.cpp
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

