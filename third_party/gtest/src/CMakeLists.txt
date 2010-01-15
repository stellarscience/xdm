project( gtest )

file( GLOB ${PROJECT_NAME}_SOURCES *.cc )

add_library( ${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_SOURCES}
)

