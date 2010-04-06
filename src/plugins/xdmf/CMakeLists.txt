project( xdmXdmfPlugin )

find_package( LibXml2 REQUIRED )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# Generate the header containing the XDMF schema.
add_custom_command( 
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/xdmf.rng
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/xdmf.rng ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/xdmf.rng
)
add_custom_target( copy_xdmf.rng ALL 
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/xdmf.rng" )

add_executable( xmlToC xmlToC.cpp )
set_target_properties( xmlToC PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

set( _exe_ext "" )
if( WIN32 )
  set( _exe_ext ".exe" )
endif()
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/XdmfRng.hpp
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/xmlToC${_exe_ext}
  ARGS XDMF_RNG_SCHEMA ${CMAKE_CURRENT_BINARY_DIR}/xdmf.rng > ${CMAKE_CURRENT_BINARY_DIR}/XdmfRng.hpp
  DEPENDS xmlToC ${CMAKE_CURRENT_BINARY_DIR}/xdmf.rng
)
add_custom_target( xdmf_rng_header 
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/XdmfRng.hpp" )
add_dependencies( xdmf_rng_header copy_xdmf.rng )

# Build the plugin
set( ${PROJECT_NAME}_HEADERS
  LinearTopologyData.hpp
  NamespaceMacro.hpp
  TemporalCollection.hpp
  TimeSeries.hpp
  VirtualDataset.hpp
  XdmfHelpers.hpp
  XmfReader.hpp
  impl/TreeBuilder.hpp
  impl/XPathQuery.hpp
)

set( ${PROJECT_NAME}_SOURCES 
  LinearTopologyData.cpp
  TemporalCollection.cpp
  VirtualDataset.cpp
  XdmfHelpers.cpp
  XmfReader.cpp
  impl/TreeBuilder.cpp
)

# Tell CMake that the RNG schema header is generated.
set_source_files_properties( 
  ${CMAKE_CURRENT_BINARY_DIR}/XdmfRng.hpp GENERATED
)

add_definitions( ${LIBXML2_DEFINITIONS} )

include_directories( 
  ${CMAKE_CURRENT_BINARY_DIR}
  ${LIBXML2_INCLUDE_DIR} )

add_library( ${PROJECT_NAME} 
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
  xdm
  xdmFormat
  xdmGrid
  xdmHdf
  ${LIBXML2_LIBRARIES}
)

add_dependencies( ${PROJECT_NAME} xdmf_rng_header )

if( BUILD_TESTING )
  add_subdirectory( test )
endif()
