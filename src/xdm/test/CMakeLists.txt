
# Add a serial test
macro( xdm_test_serial test_name )
    add_executable( xdm.${test_name}.test ${ARGN} )
    target_link_libraries( xdm.${test_name}.test xdm gtest )
    add_test( xdm.${test_name} xdm.${test_name}.test )
endmacro()

xdm_test_serial( TemplateStructuredArray TemplateStructuredArray.cpp )
xdm_test_serial( XmlObject XmlObject.cpp )
xdm_test_serial( XmlOutputStream XmlOutputStream.cpp )
xdm_test_serial( Item Item.cpp )
xdm_test_serial( DataShape DataShape.cpp )
xdm_test_serial( ThrowMacro ThrowMacro.cpp )
xdm_test_serial( UniformDataItem UniformDataItem.cpp )
xdm_test_serial( CompositeDataItem CompositeDataItem.cpp )
xdm_test_serial( CollectMetadataOperation CollectMetadataOperation.cpp )
xdm_test_serial( RefPtr RefPtr.cpp )
xdm_test_serial( ObjectCompositionMixin ObjectCompositionMixin.cpp )
xdm_test_serial( DataSelectionVisitor DataSelectionVisitor.cpp )
xdm_test_serial( WritableData WritableData.cpp )
xdm_test_serial( HyperSlabBlockIterator HyperSlabBlockIterator.cpp )

