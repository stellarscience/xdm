project( xdm )

set( ${PROJECT_NAME}_HEADERS
    CollectMetadataOperation.hpp
    CompositeDataItem.hpp
    DataItem.hpp
    Dataset.hpp
    DataShape.hpp
    HyperSlab.hpp
    Item.hpp
    ItemVisitor.hpp
	NamespaceMacro.hpp
	ObjectCompositionMixin.hpp
    ReferencedObject.hpp
    RefPtr.hpp
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
    Dataset.cpp
    DataShape.cpp
    Item.cpp
    ItemVisitor.cpp
    ReferencedObject.cpp
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

