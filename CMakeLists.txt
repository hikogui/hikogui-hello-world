# Copyright Take Vos 2021-2023.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.25)
 
#-------------------------------------------------------------------
# Enable C++20 Modules
#-------------------------------------------------------------------
#if(${CMAKE_VERSION} VERSION_LESS "3.26")
#    # CMake 3.25
#    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")
#elseif(${CMAKE_VERSION} VERSION_LESS "3.27")
#    # CMake 3.26
#    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
#else()
#    # CMake 3.27
#    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
#endif()
#set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

option(INSTALL_PDB       "Install the .pdb file along side the executable" OFF)

#-------------------------------------------------------------------
# Configure early CMAKE paths
#-------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

include(ExternalProject)

# Configure Vcpkg, including setting the `CMAKE_TOOLCHAIN_FILE` from the
# `VCPKG_ROOT` environment variable.
include(SetupVcpkg)

# Configure the compiler better for windows development,
# such that ReleaseWithDebug build produce the same assembly code as
# the Release build.
include(SetupCompiler)

# This allows us to specify component for packaging
include(CPackComponent)

# This adds functions to add resource files to a target.
include(TargetResources)


#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------

# vcpkg.json and package.hson are the primary source for version data
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json VCPKG_JSON_STRING)
string(JSON APP_NAME GET ${VCPKG_JSON_STRING} "name")
string(JSON APP_VERSION GET ${VCPKG_JSON_STRING} "version")
string(JSON APP_LICENSE GET ${VCPKG_JSON_STRING} "license")
string(JSON APP_HOMEPAGE GET ${VCPKG_JSON_STRING} "homepage")
string(JSON APP_DESCRIPTION GET ${VCPKG_JSON_STRING} "description")
configure_file("package.json.in" "package.json" @ONLY)
file(READ ${CMAKE_CURRENT_BINARY_DIR}/package.json PACKAGE_JSON_STRING)
string(JSON APP_DISPLAY_NAME GET ${PACKAGE_JSON_STRING} "display-name")
string(JSON APP_VENDOR GET ${PACKAGE_JSON_STRING} "vendor")

project(hikogui_hello_world VERSION ${APP_VERSION} LANGUAGES CXX)

#-------------------------------------------------------------------
# Find Dependencies
#-------------------------------------------------------------------
# The hikogui library and its dependencies are installed by vcpkg
# due to this `find_package()` command. The normal `find_package()`
# command was replaced by the vcpkg.cmake file, passed by the
# `CMAKE_TOOLCHAIN_FILE` variable.
find_package(hikogui REQUIRED)

#-------------------------------------------------------------------
# Define the target executable
#-------------------------------------------------------------------
# The WIN32 and MACOSX_BUNDLE parameters describe the type of executable, in this case a desktop application.
add_executable(hello_world WIN32 MACOSX_BUNDLE)

#
# This is a function that was defined by find_package(hikogui) to
# link against hikogui including its modules, hopefully not necessary in the future.
#
target_link_hikogui(hello_world PRIVATE)

# We want to use the project name and version number from the CMakeList.txt file
# to be reflected the application name and version. The configure_file
# will fill in the template file with the cmake variables.
# Since the generated current_version.hpp will be stored in the build directory
# we need add this include directory to the target.
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/metadata.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/metadata.hpp" @ONLY)

target_sources(hello_world PRIVATE "src/main.cpp")

target_resources(hello_world
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/en.po
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/nl.po
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/hello_world.png
)

#-------------------------------------------------------------------
# Installation Rules: hello_world
#-------------------------------------------------------------------
set(INSTALL_BINDIR ".")
set(INSTALL_RESOURCEDIR "resources")

install(
    TARGETS hello_world DESTINATION "${INSTALL_BINDIR}" COMPONENT hello_world
    RESOURCE DESTINATION "${INSTALL_RESOURCEDIR}" COMPONENT hello_world
)

if(INSTALL_PDB)
    install(FILES $<TARGET_PDB_FILE:hello_world> DESTINATION "." COMPONENT hello_world)
endif()

# Add files used by the package maker.
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/cpack/" DESTINATION "." COMPONENT _cpack EXCLUDE_FROM_ALL)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/package.json" DESTINATION "." COMPONENT _cpack EXCLUDE_FROM_ALL)

#-------------------------------------------------------------------
# Packaging Rules: hello_world
#-------------------------------------------------------------------

cpack_add_component(_cpack)
if(WIN32)
    set(VC_REDIST_X64 "${CMAKE_CURRENT_BINARY_DIR}/data/redist/VC_redist.x64.exe")

    install(CODE "file(DOWNLOAD \"https://aka.ms/vs/17/release/vc_redist.x64.exe\" \"${VC_REDIST_X64}\")" COMPONENT _redist EXCLUDE_FROM_ALL)

    # vcredist is needed when installing on the user's machine.
    install(PROGRAMS "${VC_REDIST_X64}"
        DESTINATION "."
        COMPONENT _redist
        EXCLUDE_FROM_ALL)

    cpack_add_component(_redist)
endif()

cpack_add_component(hello_world DISPLAY_NAME "${APP_DISPLAY_NAME}" DESCRIPTION "${APP_DESCRIPTION}" REQUIRED)

# The "External" generator will create a .json file which is used by an external package builder.
# We use the external package builder hikogui/ecpack-and-sign to create a NSIS package where
# the executables, the NSIS-uninstall executable and the NSIS-install executable all can
# be signed.
set(CPACK_GENERATOR External)
set(CPACK_EXTERNAL_ENABLE_STAGING ON)
set(CPACK_EXTERNAL_PACKAGE_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/CMake/PostStagingScript.cmake")

include(CPack)

