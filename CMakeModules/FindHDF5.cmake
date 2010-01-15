# - Find the HDF5 library.
#
# This module invokes the HDF5 wrapper compiler that should be installed
# alongside HDF5.  Depending upon the HDF5 Configuration, the wrapper compiler
# is called either h5cc or h5pcc.  If this succeeds, the module will then call
# the compiler with the -show argument to see what flags are used when compiling
# an HDF5 client application.
#
# This module will read the variable HDF5_USE_STATIC_LIBRARIES to determine
# whether or not to prefer a static link to a dynamic link for HDF5 and all of
# it's dependencies.  To use this feature, make sure that the
# HDF5_USE_STATIC_LIBRARIES variable is set before the call to find_package.
#
# This module will define the following variables:
#  HDF5_INCLUDE_DIRS - location of the hdf5 includes
#  HDF5_LIBRARIES - link libraries for HDF5
#  HDF5_FOUND - true if HDF5 was found on the system
#  HDF5_LIBRARY_DIRS - the full set of library directories
#  HDF5_COMPILER_EXECUTABLE - the path to the HDF5 wrapper compiler

include(FindPackageHandleStandardArgs)

# try to find the HDF5 wrapper compiler
find_program( HDF5_COMPILER_EXECUTABLE
    NAMES h5cc h5pcc
    DOC "HDF5 Wrapper compiler.  Used only to detect HDF5 compile flags." )
mark_as_advanced( HDF5_COMPILER_EXECUTABLE )

if( HDF5_INCLUDE_DIRS AND HDF5_LIBRARIES )
    # Do nothing: we already have HDF5_INCLUDE_PATH and HDF5_LIBRARIES in the
    # cache, it would be a shame to override them
elseif( HDF5_COMPILER_EXECUTABLE )
    exec_program( ${HDF5_COMPILER_EXECUTABLE} 
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

if( HDF5_COMPILE_CMDLINE )
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
        # The HDF wrapper compiler assumes that hdf5.h will be somewhere in your
        # sytem include path and doesn't appear to specify the location of the
        # includes.  We make a guess here that the includes may be somewhere in
        # the standard *nix layout relative to the lib directory.
        set( IPATH ${LPATH}/../include )
        list( APPEND HDF5_ALL_INCLUDE_PATHS ${IPATH} )
    endforeach()

    # now search for the library names required by HDF (match -l...)
    # match only -l's preceded by a space or comma
    # this is to exclude directory names like xxx-linux/
    string( REGEX MATCHALL "[, ]-l([^\", ]+)" HDF5_ALL_LIBRARY_FLAGS
        "${HDF5_COMPILE_CMDLINE}" )
    # strip the -l from all of the library flags and add to the search list
    # we seed the list of library names with the plain hdf5 C libraries.
    # However, we need to search for other libraries because HDF5 relies on a
    # couple of things depending upon how it was compiled.  An example of this
    # problem is when HDF is compiled with SZip support.
    set( HDF5_LIBRARY_NAMES hdf5_hl hdf5 )
    foreach( LIB ${HDF5_ALL_LIBRARY_FLAGS} )
        string( REGEX REPLACE "^[, ]-l" "" LIB ${LIB} )
        list( APPEND HDF5_LIBRARY_NAMES ${LIB} )
    endforeach()
    set( HDF5_LIBRARIES )
    foreach( LIB ${HDF5_LIBRARY_NAMES} )
        if( HDF5_USE_STATIC_LIBRARIES )
            # According to bug 1643 on the CMake bug tracker, this is the
            # preferred method for searching for a static library.
            # See http://www.cmake.org/Bug/view.php?id=1643
            set( THIS_LIBRARY_SEARCH lib${LIB}.a )
        else()
            set( THIS_LIBRARY_SEARCH ${LIB} )
        endif()
        find_library( HDF5_${LIB}_LIBRARY ${THIS_LIBRARY_SEARCH} 
            HINTS ${HDF5_LINK_PATH} )
        list( APPEND HDF5_LIBRARIES ${HDF5_${LIB}_LIBRARY} )
        mark_as_advanced( HDF5_${LIB}_LIBRARY )
    endforeach()

    # look for the include paths
    find_path( HDF5_INCLUDE_DIRS hdf5.h
        /usr/local/include
        /usr/include
        $ENV{HOME}/.local/include
        ${HDF5_ALL_INCLUDE_PATHS}
    )
endif()

mark_as_advanced( HDF5_INCLUDE_DIRS )
mark_as_advanced( HDF5_LIBRARIES )

find_package_handle_standard_args( HDF5 DEFAULT_MSG 
    HDF5_LIBRARIES 
    HDF5_INCLUDE_DIRS
)

