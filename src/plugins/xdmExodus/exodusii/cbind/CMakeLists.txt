PROJECT(ExodusII)

SET(EXODUSII_VERSION_MAJOR "2")
SET(EXODUSII_VERSION_MINOR "0")
SET(EXODUSII_VERSION_PATCH "0")
SET(EXODUSII_VERSION "${EXODUSII_VERSION_MAJOR}.${EXODUSII_VERSION_MINOR}")
SET(EXODUSII_VERSION_FULL "${EXODUSII_VERSION}.${EXODUSII_VERSION_PATCH}")

SET(EXODUSII_BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}")

INCLUDE(CheckIncludeFile)
CHECK_INCLUDE_FILE( malloc.h EX_HAVE_MALLOC_H )

CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/exodusII_cfg.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/exodusII_cfg.h
  @ONLY IMMEDIATE)

SET(cbind_SRCS
  src/exclos.c
  src/excn2s.c
  src/ex_conv.c
  src/excopy.c
  src/excre.c
  src/exerr.c
  src/exgatm.c
  src/exgatn.c
  src/exgatt.c
  src/exgattp.c
  src/exgblk.c
  src/exgcns.c
  src/exgcon.c
  src/exgconn.c
  src/exgcor.c
  src/exgcset.c
  src/exgcss.c
  src/exgcssc.c
  src/exgean.c
  src/exgeat.c
  src/exgebi.c
  src/exgecpp.c
  src/exgelb.c
  src/exgelc.c
  src/exgem.c
  src/exgenm.c
  src/exgev.c
  src/exgevid.c
  src/exgevt.c
  src/exgfrm.c
  src/exggv.c
  src/exggvt.c
  src/exgidm.c
  src/exgids.c
  src/exginf.c
  src/exgini.c
  src/exginix.c
  src/exgmap.c
  src/exgmp.c
  src/exgnam.c
  src/exgnams.c
  src/exgncor.c
  src/exgnmap.c
  src/exgnm.c
  src/exgnnm.c
  src/exgnnv.c
  src/exgnoatt.c
  src/exgnp.c
  src/exgns.c
  src/exgnsd.c
  src/exgnsi.c
  src/exgnstt.c
  src/exgnsv.c
  src/exgnsvid.c
  src/exgnvar.c
  src/exgnv.c
  src/exgnvid.c
  src/exgnvt.c
  src/exgnvv.c
  src/exgoatt.c
  src/exgoea.c
  src/exgotv.c
  src/exgpa.c
  src/exgp.c
  src/exgpem.c
  src/exgpn.c
  src/exgqa.c
  src/exgset.c
  src/exgsetd.c
  src/exgsetp.c
  src/exgsnl.c
  src/exgsp.c
  src/exgss.c
  src/exgssc.c
  src/exgssd.c
  src/exgssi.c
  src/exgssn.c
  src/exgsstt.c
  src/exgssv.c
  src/exgssvid.c
  src/exgtim.c
  src/exgtt.c
  src/exgvan.c
  src/exgvar.c
  src/exgvarnam.c
  src/exgvarnams.c
  src/exgvarparam.c
  src/exgvartab.c
  src/exgvart.c
  src/exgvid.c
  src/exgvnm.c
  src/exgvp.c
  src/exgvtt.c
  src/exgvv.c
  src/exinq.c
  src/exopen.c
  src/exopts.c
  src/expatn.c
  src/expatt.c
  src/expattp.c
  src/expblk.c
  src/expcab.c
  src/expclb.c
  src/expcns.c
  src/expcon.c
  src/expconn.c
  src/expcor.c
  src/expcset.c
  src/expcss.c
  src/expean.c
  src/expeat.c
  src/expecpp.c
  src/expelb.c
  src/expelc.c
  src/expem.c
  src/expenm.c
  src/expev.c
  src/expfrm.c
  src/expgv.c
  src/expidm.c
  src/expinf.c
  src/expini.c
  src/expinix.c
  src/expmap.c
  src/expmp.c
  src/expnam.c
  src/expnams.c
  src/expncor.c
  src/expnmap.c
  src/expnm.c
  src/expnnm.c
  src/expnnv.c
  src/expnoatt.c
  src/expnp.c
  src/expns.c
  src/expnsd.c
  src/expnstt.c
  src/expnsv.c
  src/expnvar.c
  src/expnv.c
  src/expnvv.c
  src/expoatt.c
  src/expoea.c
  src/exppa.c
  src/expp.c
  src/exppem.c
  src/exppn.c
  src/exppsetd.c
  src/expqa.c
  src/expset.c
  src/expsetd.c
  src/expsetp.c
  src/expsp.c
  src/expss.c
  src/expssd.c
  src/expsstt.c
  src/expssv.c
  src/exptim.c
  src/exptt.c
  src/expvan.c
  src/expvar.c
  src/expvarnam.c
  src/expvarnams.c
  src/expvarparam.c
  src/expvartab.c
  src/expvnm.c
  src/expvpa.c
  src/expvpax.c
  src/expvp.c
  src/expvpc.c
  src/expvtt.c
  src/expvv.c
  src/exupda.c
  src/ex_utils.c
)

SET(cbind_HEADERS
  include/exodusII.h
  include/exodusII_ext.h
  include/exodusII_int.h
)

IF(APPLE)
  SET_SOURCE_FILES_PROPERTIES(
    src/ex_utils.c
    PROPERTIES
    COMPILE_FLAGS -fno-common)
ENDIF(APPLE)

ADD_LIBRARY(exoIIv2c ${cbind_SRCS})

TARGET_LINK_LIBRARIES( exoIIv2c ${NETCDF_LIBRARY} )

IF(VTK_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(exoIIv2c PROPERTIES ${VTK_LIBRARY_PROPERTIES})
ENDIF(VTK_LIBRARY_PROPERTIES)

IF(NOT VTK_INSTALL_NO_LIBRARIES)
#MESSAGE("cbind_HEADERS=${cbind_HEADERS}")
  IF(VTK_INSTALL_HAS_CMAKE_24)
    INSTALL(TARGETS exoIIv2c
      RUNTIME DESTINATION /lib COMPONENT Runtime
      LIBRARY DESTINATION /lib COMPONENT Runtime
      ARCHIVE DESTINATION /lib COMPONENT Development)
    INSTALL(FILES ${cbind_HEADERS} DESTINATION /include)
  ELSE(VTK_INSTALL_HAS_CMAKE_24)
    INSTALL_TARGETS(/lib exoIIv2c)
    INSTALL_FILES(/include .h ${cbind_HEADERS})
  ENDIF(VTK_INSTALL_HAS_CMAKE_24)
ENDIF(NOT VTK_INSTALL_NO_LIBRARIES)

IF(NOT VTK_INSTALL_NO_DEVELOPMENT)
#  INSTALL_FILES(${VTK_INSTALL_INCLUDE_DIR}/vtkexodus2 .h
ENDIF(NOT VTK_INSTALL_NO_DEVELOPMENT)

INCLUDE( CTest )

IF (BUILD_TESTING)
  SUBDIRS(test)
ENDIF (BUILD_TESTING)

