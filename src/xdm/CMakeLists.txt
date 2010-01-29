project( xdm )

set( ${PROJECT_NAME}_HEADERS
    Algorithm.hpp
    AllDataSelection.hpp
    BinaryIosBase.hpp
    BinaryIStream.hpp
    BinaryIOStream.hpp
    BinaryOStream.hpp
    BinaryStreamBuffer.hpp
    BinaryStreamOperations.hpp
    ByteArray.hpp
    CollectMetadataOperation.hpp
    CompositeDataItem.hpp
    CoordinateDataSelection.hpp
    DataItem.hpp
    DataSelection.hpp
    DataSelectionMap.hpp
    DataSelectionVisitor.hpp
    Dataset.hpp
    DatasetExcept.hpp
    DataShape.hpp
    HyperSlab.hpp
    HyperSlabBlockIterator.hpp
    HyperslabDataSelection.hpp
    Item.hpp
    ItemVisitor.hpp
	  NamespaceMacro.hpp
	  ObjectCompositionMixin.hpp
    PrimitiveType.hpp
    ProxyDataset.hpp
    ReferencedObject.hpp
    ReferenceVector.hpp
    RefPtr.hpp
    SelectableDataMixin.hpp
    SerializeDataOperation.hpp
    StaticAssert.hpp
    StructuredArray.hpp
    TemplateStructuredArray.hpp
    UniformDataItem.hpp
    UpdateVisitor.hpp
    VectorStructuredArray.hpp
    WritableArray.hpp
    WritableData.hpp
    XmlExcept.hpp
    XmlMetadataWrapper.hpp
    XmlObject.hpp
    XmlOutputStream.hpp
    XmlTextContent.hpp
)

set( ${PROJECT_NAME}_SOURCES
    BinaryIStream.cpp
    BinaryIOStream.cpp
    BinaryOStream.cpp
    BinaryStreamOperations.cpp
    CollectMetadataOperation.cpp
    CompositeDataItem.cpp
    CoordinateDataSelection.cpp
    DataItem.cpp
    DataSelection.cpp
    DataSelectionMap.cpp
    DataSelectionVisitor.cpp
    Dataset.cpp
    DataShape.cpp
    Item.cpp
    ItemVisitor.cpp
    PrimitiveType.cpp
    ProxyDataset.cpp
    ReferencedObject.cpp
    ReferenceVector.cpp
    SelectableDataMixin.cpp
    SerializeDataOperation.cpp
    StructuredArray.cpp
    UniformDataItem.cpp
    UpdateVisitor.cpp
    WritableArray.cpp
    WritableData.cpp
    XmlObject.cpp
    XmlOutputStream.cpp
)

add_library( ${PROJECT_NAME} 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME} )

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

