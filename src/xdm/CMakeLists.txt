project( xdm )

set( ${PROJECT_NAME}_HEADERS
    Algorithm.hpp
    AllDataSelection.hpp
    ArrayAdapter.hpp
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
    DataIndexingVisitor.hpp
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
    MemoryAdapter.hpp
	  NamespaceMacro.hpp
	  ObjectCompositionMixin.hpp
    PrimitiveType.hpp
    ProxyDataset.hpp
    ReferencedObject.hpp
    RefPtr.hpp
    SelectableDataMixin.hpp
    SerializeDataOperation.hpp
    StaticAssert.hpp
    StructuredArray.hpp
    TypedDataIndexingVisitor.hpp
    TypedStructuredArray.hpp
    UniformDataItem.hpp
    UpdateVisitor.hpp
    VectorRef.hpp
    VectorStructuredArray.hpp
    XmlExcept.hpp
    XmlMetadataWrapper.hpp
    XmlObject.hpp
    XmlOutputStream.hpp
    XmlTextContent.hpp
)

set( ${PROJECT_NAME}_SOURCES
    ArrayAdapter.cpp
    BinaryIStream.cpp
    BinaryIOStream.cpp
    BinaryOStream.cpp
    BinaryStreamBuffer.cpp
    BinaryStreamOperations.cpp
    CollectMetadataOperation.cpp
    CompositeDataItem.cpp
    CoordinateDataSelection.cpp
    DataIndexingVisitor.cpp
    DataItem.cpp
    DataSelection.cpp
    DataSelectionMap.cpp
    DataSelectionVisitor.cpp
    Dataset.cpp
    DataShape.cpp
    Item.cpp
    ItemVisitor.cpp
    MemoryAdapter.cpp
    PrimitiveType.cpp
    ProxyDataset.cpp
    ReferencedObject.cpp
    SelectableDataMixin.cpp
    SerializeDataOperation.cpp
    StructuredArray.cpp
    UniformDataItem.cpp
    UpdateVisitor.cpp
    VectorRef.cpp
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

