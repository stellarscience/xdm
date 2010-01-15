project( xdm )

set( ${PROJECT_NAME}_HEADERS
    AllDataSelection.hpp
    CollectMetadataOperation.hpp
    CompositeDataItem.hpp
    DataItem.hpp
    DataSelection.hpp
    DataSelectionVisitor.hpp
    Dataset.hpp
    DataShape.hpp
    HyperSlab.hpp
    HyperslabDataSelection.hpp
    Item.hpp
    ItemVisitor.hpp
	NamespaceMacro.hpp
	ObjectCompositionMixin.hpp
    ReferencedObject.hpp
    RefPtr.hpp
    SelectableDataMixin.hpp
    SerializeDataOperation.hpp
    StructuredArray.hpp
    TemplateStructuredArray.hpp
    UniformDataItem.hpp
    XmlObject.hpp
    XmlOutputStream.hpp
)

set( ${PROJECT_NAME}_SOURCES
    CollectMetadataOperation.cpp
    CompositeDataItem.cpp
    DataItem.cpp
    DataSelection.cpp
    DataSelectionVisitor.cpp
    Dataset.cpp
    DataShape.cpp
    Item.cpp
    ItemVisitor.cpp
    ReferencedObject.cpp
    SelectableDataMixin.cpp
    SerializeDataOperation.cpp
    StructuredArray.cpp
    UniformDataItem.cpp
    XmlObject.cpp
    XmlOutputStream.cpp
)

add_library( ${PROJECT_NAME} 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME} )

add_subdirectory( test )

