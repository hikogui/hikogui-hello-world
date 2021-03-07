# Copyright Take Vos 2021.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.19)

#-------------------------------------------------------------------
# Configure early CMAKE paths
#-------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
include(SetupVcpkg)

#-------------------------------------------------------------------
# Compiler Setup
#-------------------------------------------------------------------

# Define C++ Standard to use
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(WIN32)
    # Build for a Windows 10 host system.
    set(CMAKE_SYSTEM_VERSION 10.0)

    message(STATUS "[INFO] BUILD_SHARED_LIBS -> '${BUILD_SHARED_LIBS}'.")

    # When we build statically (MT):
    if(NOT BUILD_SHARED_LIBS)
        # Select MSVC runtime based on CMAKE_MSVC_RUNTIME_LIBRARY.
        # We switch from the multi-threaded dynamically-linked library (default)
        # to the multi-threaded statically-linked runtime library.
        cmake_policy(SET CMP0091 NEW)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()


    # Global Compiler flags for optimized Release and RelWithDebugInfo builds
    # All overrides are for multi-threaded dynamically-linked libs: MD + MDd.
    if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        set(CMAKE_USER_MAKE_RULES_OVERRIDE "CMakeOverride.txt")
    endif()

endif()

#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------
project(ttauri_hello_world VERSION 0.1.0 LANGUAGES CXX)

#-------------------------------------------------------------------
# Find Dependencies
#-------------------------------------------------------------------
find_package(ttauri REQUIRED)

add_executable(hello_world WIN32 MACOSX_BUNDLE)
target_link_libraries(hello_world PRIVATE ttauri::ttauri)
target_sources(hello_world PRIVATE src/main.cpp)

# Copy library resource files to application.
add_custom_target(hello_world_resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ttauri_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
  DEPENDS
    resources/locale/en.po
    resources/locale/nl.po
    resources/hello_world.png
  )

add_dependencies(hello_world hello_world_resources)
