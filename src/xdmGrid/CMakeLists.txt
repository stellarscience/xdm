project( xdmGrid )

set( ${PROJECT_NAME}_HEADERS 
    Attribute.hpp
    Cell.hpp
    CollectionGrid.hpp
    Domain.hpp
    Forward.hpp
    Geometry.hpp
    Grid.hpp
    InterlacedGeometry.hpp
    MultiArrayGeometry.hpp
    NamespaceMacro.hpp
    Polyvertex.hpp
    RectilinearMesh.hpp
    ReferencingGrid.hpp
    StructuredTopology.hpp
    TensorProductGeometry.hpp
    Time.hpp
    Topology.hpp
    UniformGrid.hpp
    UnstructuredTopology.hpp
    UnstructuredTopologyConventions.hpp
)

set( ${PROJECT_NAME}_SOURCES
    Attribute.cpp
    Cell.cpp
    CollectionGrid.cpp
    Domain.cpp
    Geometry.cpp
    Grid.cpp
    InterlacedGeometry.cpp
    MultiArrayGeometry.cpp
    Polyvertex.cpp
    RectilinearMesh.cpp
    ReferencingGrid.cpp
    StructuredTopology.cpp
    TensorProductGeometry.cpp
    Time.cpp
    Topology.cpp
    UniformGrid.cpp
    UnstructuredTopology.cpp
    UnstructuredTopologyConventions.cpp
)

add_library( ${PROJECT_NAME} STATIC 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
    xdm
)

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

