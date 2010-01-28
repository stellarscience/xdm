project( xdmComm )

find_package( MPI REQUIRED )

set( ${PROJECT_NAME}_HEADERS 
    BarrierOnExit.hpp
    CoalescingStreamBuffer.hpp
    DistributedItemCollectionProxy.hpp
    MpiDatasetProxy.hpp
    NamespaceMacro.hpp
    ParallelizeTreeVisitor.hpp
    RankOrderedDistributedDataset.hpp
)

set( ${PROJECT_NAME}_SOURCES
    BarrierOnExit.cpp
    CoalescingStreamBuffer.cpp
    DistributedItemCollectionProxy.cpp
    MpiDatasetProxy.cpp
    ParallelizeTreeVisitor.cpp
    RankOrderedDistributedDataset.cpp
)

add_definitions( 
    ${MPI_COMPILE_FLAGS}
)

include_directories(
    ${MPI_INCLUDE_PATH}
)

add_library( ${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
    xdm
    ${MPI_LIBRARIES}
)

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

