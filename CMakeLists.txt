# Copyright 2014 Jetperch LLC.  All rights reserved.
# libusbk_examples

cmake_minimum_required (VERSION 2.8) 
project (libusbk_examples)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
SET(VERSION_UNDERSCORE "${VERSION_MAJOR}_${VERSION_MINOR}_${VERSION_PATCH}")


# Find libusbk
set(LIBUSBK_DIR "" CACHE PATH "The path to the libusbk binary distribution.")
if (NOT EXISTS ${LIBUSBK_DIR})
    message(FATAL_ERROR "libusbk not found at ${LIBUSBK_DIR}.  Please specify LIBUSBK_DIR")
endif()
set(LIBUSBK_INCLUDE "${LIBUSBK_DIR}/includes")
if (MINGW)
    set(LIBUSBK_LINK ${LIBUSBK_DIR}/bin/lib/x86)
    set(LIBUSBK_LIBS libusbK)
    set(CMAKE_CXX_FLAGS "-std=gnu++0x")
elseif (${CMAKE_SIZEOF_VOID_P} STREQUAL 4)
    set(LIBUSBK_LINK ${LIBUSBK_DIR}/bin/lib/static/x86)
    set(LIBUSBK_LIBS libusbK Setupapi)
elseif (${CMAKE_SIZEOF_VOID_P} STREQUAL 8)
    set(LIBUSBK_LINK ${LIBUSBK_DIR}/bin/lib/static/amd64)
    set(LIBUSBK_LIBS libusbK Setupapi)
else()
    message(FATAL_ERROR "Unsupported target architecture")
endif()

include_directories(${LIBUSBK_INCLUDE})
link_directories(${LIBUSBK_LINK})
add_definitions(-D__STDC_LIMIT_MACROS)

add_subdirectory(src)


# -- CPACK Configuration --
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libusbk_examples")
SET(CPACK_PACKAGE_VENDOR "Jetperch LLC")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
INCLUDE(CPack)
