#
# Find the HDF5 library
#
# This module will set the following variables:
# - HDF5_FOUND true if HDF5 was found on the system
# - HDF5_INCLUDE_PATH location of HDF5 includes
# - HDF5_LIBRARIES link libraries for HDF5

include(FindPackageHandleStandardArgs)

# try to find the HDF5 wrapper compiler
find_program( HDF5_COMPILER
    NAMES h5cc
    DOC "HDF5 Wrapper compiler.  Used only to detect HDF5 compile flags." )
mark_as_advanced( HDF5_COMPILER )

if( HDF5_INCLUDE_PATH AND HDF5_LIBRARIES )
    # Do nothing: we already have HDF5_INCLUDE_PATH and HDF5_LIBRARIES in the
    # cache, it would be a shame to override them
elseif( HDF5_COMPILER )
    exec_program( ${HDF5_COMPILER} 
        ARGS -show
        OUTPUT_VARIABLE HDF5_COMPILE_CMDLINE
        RETURN_VALUE HDF5_COMPILER_RETURN
    )
    if( HDF5_COMPILER_RETURN EQUAL 0 )
        # do nothing
    else()
        message( STATUS "Unable to determine HDF5 from HDF5 wrapper." )
    endif()
endif()

if( HDF5_INCLUDE_PATH AND HDF5_LIBRARIES )
    # do nothing
elseif( HDF5_COMPILE_CMDLINE )
    # grab the hdf5 libraries from the command line.  We will use the path of
    # these things to find the include dirs
    # First: linker paths
    string( REGEX MATCHALL "-L([^\" ]+|\"[^\"]+\")" HDF5_ALL_LINK_PATHS
        "${HDF5_COMPILE_CMDLINE}"
    )
    set( HDF5_LINK_PATH )
    set( HDF5_ALL_INCLUDE_PATHS )
    foreach( LPATH ${HDF5_ALL_LINK_PATHS} )
        string( REGEX REPLACE "^-L" "" LPATH ${LPATH} )
        string( REGEX REPLACE "//" "/" LPATH ${LPATH} )
        list( APPEND HDF5_LINK_PATH ${LPATH} )
        # string( REGEX REPLACE "lib" "include" LPATH ${IPATH} )
        list( APPEND HDF5_ALL_INCLUDE_PATHS ${IPATH} )
    endforeach()

    set( HDF5_LIBRARY_NAMES hdf5 hdf5_hl )
    set( HDF5_LIBRARIES )
    foreach( LIB ${HDF5_LIBRARY_NAMES} )
        find_library( HDF5_${LIB}_LIBRARY ${LIB} HINTS ${HDF5_LINK_PATH} )
        list( APPEND HDF5_LIBRARIES ${HDF5_${LIB}_LIBRARY} )
    endforeach()

    # look for the include paths
    find_path( HDF5_INCLUDE_PATH hdf5.h
        /usr/local/include
        /usr/include
        $ENV{HOME}/.local/include
        ${HDF5_ALL_INCLUDE_PATHS}
    )
endif()

find_package_handle_standard_args( HDF5 DEFAULT_MSG 
    HDF5_LIBRARIES 
    HDF5_INCLUDE_PATH
)



