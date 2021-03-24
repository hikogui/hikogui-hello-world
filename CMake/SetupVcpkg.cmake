#
# SetupVcpkg
#
# This configures vcpkg using environment variables instead of a command-line options.
#
# https://github.com/microsoft/vcpkg/blob/master/docs/users/integration.md#using-an-environment-variable-instead-of-a-command-line-option
#
# Environment Variables: https://vcpkg.readthedocs.io/en/latest/users/config-environment/
#

set(VCPKG_ENABLED on)

if(DEFINED ENV{VCPKG_VERBOSE} AND NOT DEFINED VCPKG_VERBOSE)
    set(VCPKG_VERBOSE "$ENV{VCPKG_VERBOSE}" CACHE BOOL "")
endif()

#
# -- Automatic install of vcpkg dependencies.
#
# This is experimental.
# See https://github.com/Microsoft/vcpkg/issues/1653
#

set(VCPKG_APPLOCAL_DEPS_INSTALL ON)

# Copy dependencies into the output directory for executables.
if(DEFINED ENV{VCPKG_APPLOCAL_DEPS} AND NOT DEFINED VCPKG_APPLOCAL_DEPS)
    set(VCPKG_APPLOCAL_DEPS "$ENV{VCPKG_APPLOCAL_DEPS}" CACHE BOOL "")
endif()

# Copy dependencies into the install target directory for executables.
if(DEFINED ENV{X_VCPKG_APPLOCAL_DEPS_INSTALL} AND NOT DEFINED VCPKG_APPLOCAL_DEPS)

    # X_VCPKG_APPLOCAL_DEPS_INSTALL depends on CMake policy CMP0087
    if(POLICY CMP0087)
        cmake_policy(SET CMP0087 NEW)
    endif()
    
    set(X_VCPKG_APPLOCAL_DEPS_INSTALL "$ENV{X_VCPKG_APPLOCAL_DEPS_INSTALL}" CACHE BOOL "")
endif()

#
# --  VCPKG_ROOT
#
# Please set VCPKG_ROOT on your env: export VCPKG_ROOT=/opt/vcpkg/bin
# This avoids passing it on the configure line: -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
#
if (DEFINED CMAKE_TOOLCHAIN_FILE)
    if (NOT DEFINED ENV{VCPKG_ROOT})
        string(REPLACE "/scripts/buildsystems/vcpkg.cmake" "" _VCPKG_ROOT "${CMAKE_TOOLCHAIN_FILE}")
        set(ENV{VCPKG_ROOT} "${_VCPKG_ROOT}")
    endif()
elseif (DEFINED ENV{VCPKG_ROOT})
    if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    endif()
else()
    message(FATAL_ERROR "One of -DCMAKE_TOOLCHAIN_FILE or the VCPKG_ROOT environment variable must be set.")
endif()

#
# -- VCPKG_FEATURE_FLAGS
#
# This env var can be set to a comma-separated list of off-by-default features in vcpkg.
#
# Available features are: manifests, binarycaching.
#
# manifests     -> use the project-local manifest file "vcpkg.json" to build dependencies
# binarycaching -> use prebuild packages from cache to avoid rebuilds
#
# https://vcpkg.readthedocs.io/en/latest/specifications/manifests/
# https://vcpkg.readthedocs.io/en/latest/specifications/binarycaching/
#
if(NOT DEFINED VCPKG_FEATURE_FLAGS)
    set(VCPKG_FEATURE_FLAGS "manifests")
endif()

#
# -- VCPKG_TARGET_TRIPLET
#
# A triplet defines the build target environment in a compact string.
# [target-architecture]-[platform]-[linkage type]
# Examples: x86-windows, x64-windows-static, x64-linux.
#
# https://vcpkg.readthedocs.io/en/latest/users/triplets/
#
# The VCPKG_DEFAULT_TRIPLET is automatically set by vcpkg.cmake.
# The default triplets are: Windows: x86-windows, Linux: x64-linux, OSX: x64-osx.
#
# If you want to build for other platforms, e.g. build for Linux on Windows-x64 (canadian-cross builds)
# please set VCPKG_TARGET_TRIPLET as env var: export VCPKG_TARGET_TRIPLET=x64-linux
#
if(DEFINED ENV{VCPKG_TARGET_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_TARGET_TRIPLET}" CACHE STRING "")
endif()

#
# -- VCPKG_DEFAULT_TRIPLET
#
if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

iF(NOT DEFINED VCPKG_MANIFEST_FILE)
    set(VCPKG_MANIFEST_FILE "${CMAKE_SOURCE_DIR}/vcpkg.json")
endif()

# Add this file and the VCPKG_MANIFEST_FILE as a "vcpkg" source_group to the IDE.
# They are not automatically picked up and listed as "important project" files by IDEs, yet.
source_group("vcpkg" FILES
    "${CMAKE_SOURCE_DIR}/cmake/SetupVcpkg.cmake"
    "${CMAKE_SOURCE_DIR}/vcpkg.json"
)

#
# Set the BUILD_SHARED_LIBS based on the triplet if not set.
#
if(NOT DEFINED ENV{BUILD_SHARED_LIBS})
    if ("${VCPKG_TARGET_TRIPLET}" MATCHES ".*-static")
        set(BUILD_SHARED_LIBS "NO")
    else()
        set(BUILD_SHARED_LIBS "NO")
    endif()
endif()

#
# Print VCPKG configuration overview
#
message(STATUS "")
message(STATUS "[VCPKG]  Configuration Overview:")
message(STATUS "")
message(STATUS "[VCPKG]  VCPKG_VERBOSE                 -> '${VCPKG_VERBOSE}'")
message(STATUS "[VCPKG]  VCPKG_APPLOCAL_DEPS           -> '${VCPKG_APPLOCAL_DEPS}'")
message(STATUS "[VCPKG]  VCPKG_FEATURE_FLAGS           -> '${VCPKG_FEATURE_FLAGS}'")
message(STATUS "[VCPKG]  VCPKG_ROOT                    -> '$ENV{VCPKG_ROOT}'")
message(STATUS "[VCPKG]  VCPKG_MANIFEST_FILE           -> '${VCPKG_MANIFEST_FILE}'")
message(STATUS "[VCPKG]  VCPKG_TARGET_TRIPLET          -> '${VCPKG_TARGET_TRIPLET}'")
message(STATUS "[VCPKG]  CMAKE_TOOLCHAIN_FILE          -> '${CMAKE_TOOLCHAIN_FILE}'")
message(STATUS "[VCPKG]  BUILD_SHARED_LIBS             -> '${BUILD_SHARED_LIBS}'")
message(STATUS "")

#
# Check to make sure the VCPKG_TARGET_TRIPLET matches BUILD_SHARED_LIBS
#
if ("${VCPKG_TARGET_TRIPLET}" MATCHES ".*-static")
    if (BUILD_SHARED_LIBS)
        message(FATAL_ERROR "When the VCPKG_TARGET_TRIPLET ends with '-static' the BUILD_SHARED_LIBS must be 'OFF'.")
    endif()
else()
    if (NOT BUILD_SHARED_LIBS)
        message(FATAL_ERROR "When the VCPKG_TARGET_TRIPLET does not end with '-static' the BUILD_SHARED_LIBS must be 'ON'.")
    endif()
endif()

function(vcpkg_uninstall package)
    if(WIN32)
        set(VCPKG_EXECUTABLE "$ENV{VCPKG_ROOT}/vcpkg.exe")
    else()
        set(VCPKG_EXECUTABLE "$ENV{VCPKG_ROOT}/vcpkg")
    endif()

    set(VCPKG_INSTALL_ROOT "${CMAKE_BINARY_DIR}/vcpkg_installed")

    message(STATUS "Running vcpkg remove '${package}'.")
    execute_process(
        COMMAND "${VCPKG_EXECUTABLE}"
        "--vcpkg-root=$ENV{VCPKG_ROOT}"
        "--x-install-root=${VCPKG_INSTALL_ROOT}"
        "--triplet=${VCPKG_TARGET_TRIPLET}"
        "remove" "${package}")
endfunction()