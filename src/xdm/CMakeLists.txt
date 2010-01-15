project( xdmf )

set( ${PROJECT_NAME}_HEADERS 
    Data.hpp
	DataItem.hpp
	Domain.hpp
	Geometry.hpp
	Grid.hpp
	Group.hpp
	Item.hpp
	ItemVisitor.hpp
	NamespaceMacro.hpp
	Root.hpp
	Topology.hpp
)
set( ${PROJECT_NAME}_SOURCES 
    Data.cpp
	DataItem.cpp
	Domain.cpp
	Geometry.cpp
	Grid.cpp
	Group.cpp
	Item.cpp
	ItemVisitor.cpp
	Root.cpp
	Topology.cpp
)

add_library( ${PROJECT_NAME} 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

