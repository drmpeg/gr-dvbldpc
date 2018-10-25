INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_DVBLDPC dvbldpc)

FIND_PATH(
    DVBLDPC_INCLUDE_DIRS
    NAMES dvbldpc/api.h
    HINTS $ENV{DVBLDPC_DIR}/include
        ${PC_DVBLDPC_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DVBLDPC_LIBRARIES
    NAMES gnuradio-dvbldpc
    HINTS $ENV{DVBLDPC_DIR}/lib
        ${PC_DVBLDPC_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DVBLDPC DEFAULT_MSG DVBLDPC_LIBRARIES DVBLDPC_INCLUDE_DIRS)
MARK_AS_ADVANCED(DVBLDPC_LIBRARIES DVBLDPC_INCLUDE_DIRS)

