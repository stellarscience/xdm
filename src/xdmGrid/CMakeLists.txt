project( xdmGrid )

set( ${PROJECT_NAME}_HEADERS 
    Domain.hpp
    Geometry.hpp
    Grid.hpp
    NamespaceMacro.hpp
    RectilinearMesh.hpp
    StructuredTopology.hpp
    TensorProductGeometry.hpp
    Topology.hpp
)

set( ${PROJECT_NAME}_SOURCES
    Domain.cpp
    Geometry.cpp
    Grid.cpp
    RectilinearMesh.hpp
    StructuredTopology.cpp
    TensorProductGeometry.cpp
    Topology.cpp
)

add_library( ${PROJECT_NAME} STATIC 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
    xdm
)

add_subdirectory( test )

