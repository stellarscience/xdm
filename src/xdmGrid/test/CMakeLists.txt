
macro( xdmGrid_test_serial test_name )
    add_executable( ${test_name} ${ARGN} )
    target_link_libraries( ${test_name} xdmGrid gtest )
    add_test( ${test_name} ${test_name} )
endmacro()

xdmGrid_test_serial( Grid Grid.cpp )
xdmGrid_test_serial( UniformGrid UniformGrid.cpp )
xdmGrid_test_serial( Attribute Attribute.cpp )
xdmGrid_test_serial( CollectionGrid CollectionGrid.cpp )
xdmGrid_test_serial( Domain Domain.cpp )
xdmGrid_test_serial( Geometry Geometry.cpp )
xdmGrid_test_serial( RectilinearMesh RectilinearMesh.cpp )
xdmGrid_test_serial( StructuredTopology StructuredTopology.cpp )
xdmGrid_test_serial( TensorProductGeometry TensorProductGeometry.cpp )
xdmGrid_test_serial( Topology Topology.cpp )
xdmGrid_test_serial( Time Time.cpp )

