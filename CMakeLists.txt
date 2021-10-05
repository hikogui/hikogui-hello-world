# Copyright Take Vos 2021.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.19)
option(BUILD_SHARED_LIBS    "Build shared libraries"            OFF)
option(TTAURI_MAIN          "Build main branch of ttauri."      OFF)
option(TT_BUILD_EXAMPLES    "Build example applications"        OFF)
option(TT_BUILD_TESTS       "Build tests"                       OFF)
option(TT_BUILD_PCH         "Build precompiled headers"          ON)
option(TT_INSTALL           "Generate installation target"      OFF)
option(TT_ENABLE_ANALYSIS   "Compile using -analyze"            OFF)
option(TT_ENABLE_ASAN       "Compile using address sanitizer"   OFF)
#-------------------------------------------------------------------
# Configure early CMAKE paths
#-------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

include(FetchContent)

# Configure Vcpkg, including setting the `CMAKE_TOOLCHAIN_FILE` from the
# `VCPKG_ROOT` environment variable.
include(SetupVcpkg)

# Configure the compiler better for windows development,
# such that ReleaseWithDebug build produce the same assembly code as
# the Release build.
include(SetupCompiler)

# This allows us to specify component for packaging
include(CPackComponent)

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

# The ttauri hello world application is used for testing the development of ttauri.
# This option will make sure that:
# - TTauri is uninstalled each time cmake is run.
# - Will download the ttauri sources freshly (-binarycaching).
# - Will use the overlay port to get ttauri's main-branch.
if(TTAURI_MAIN)
    if(VCPKG_TOOLCHAIN)
        set(VCPKG_FEATURE_FLAGS "manifests,-binarycaching")
        set(VCPKG_OVERLAY_PORTS "${CMAKE_SOURCE_DIR}/vcpkg-overlay-ports")
        vcpkg_uninstall(ttauri)
    endif()
    set(TTAURI_TAG "main")
else()
    set(TTAURI_TAG "v${APP_VERSION}")
endif()

project(${APP_NAME} VERSION ${APP_VERSION} LANGUAGES CXX)

#-------------------------------------------------------------------
# Find Dependencies
#-------------------------------------------------------------------
# The ttauri library and its dependencies are installed by vcpkg
# due to this `find_package()` command. The normal `find_package()`
# command was replaced by the vcpkg.cmake file, passed by the
# `CMAKE_TOOLCHAIN_FILE` variable. The `CMAKE_TOOLCHAIN_FILE` was
# set by the include(SetupVcpkg) earlier in this file.
if(VCPKG_TOOLCHAIN)
    # Version is latest vcpkg release, or 'main' from the vcpkg overlay.
    find_package(ttauri REQUIRED)
else()
    FetchContent_Declare(ttauri GIT_REPOSITORY https://github.com/ttauri-project/ttauri.git GIT_TAG "${TTAURI_TAG}")
    FetchContent_MakeAvailable(ttauri)
    set(ttauri_DIR ${ttauri_BINARY_DIR})
endif()

#-------------------------------------------------------------------
# Define the target executable
#-------------------------------------------------------------------
# The WIN32 and MACOSX_BUNDLE parameters describe the type of executable, in this case a desktop application.
add_executable(hello_world WIN32 MACOSX_BUNDLE)

target_link_libraries(hello_world PRIVATE ttauri::ttauri)

# We want to use the project name and version number from the CMakeList.txt file
# to be reflected the application name and version. The configure_file
# will fill in the template file with the cmake variables.
# Since the generated current_version.hpp will be stored in the build directory
# we need add this include directory to the target.
configure_file("src/metadata.hpp.in" "src/metadata.hpp" @ONLY)
target_include_directories(hello_world PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/src")

target_sources(hello_world PRIVATE "src/main.cpp")

# The resources needed by the ttauri library, and the resources needed for this application
# are both copied into the same resources directory next to the executable.
add_custom_target(hello_world_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ttauri_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources"
    DEPENDS
        ttauri
        "resources/locale/en.po"
        "resources/locale/nl.po"
        "resources/hello_world.png"
)
add_dependencies(hello_world hello_world_resources)

#-------------------------------------------------------------------
# Installation Rules: hello_world
#-------------------------------------------------------------------

install(TARGETS hello_world DESTINATION "." COMPONENT hello_world)

# Copy the resources from ttauri and from the hello world application
# to the install directory.
install(DIRECTORY "${ttauri_DIR}/resources/" DESTINATION "resources" COMPONENT hello_world)
install(DIRECTORY "resources/" DESTINATION "resources" COMPONENT hello_world)

# Add files used by the package maker.
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/cpack/" DESTINATION "." COMPONENT _cpack EXCLUDE_FROM_ALL)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/package.json" DESTINATION "." COMPONENT _cpack EXCLUDE_FROM_ALL)

if(WIN32)
    # vcredist is needed when installing on the user's machine.
    install(PROGRAMS "${CMAKE_CURRENT_SOURCE_DIR}/data/redist/VC_redist.x64.exe"
        DESTINATION "."
        COMPONENT _redist
        EXCLUDE_FROM_ALL)
endif()


#-------------------------------------------------------------------
# Packaging Rules: hello_world
#-------------------------------------------------------------------

cpack_add_component(_cpack)
if(WIN32)
    cpack_add_component(_redist)
endif()

cpack_add_component(hello_world DISPLAY_NAME "${APP_DISPLAY_NAME}" DESCRIPTION "${APP_DESCRIPTION}" REQUIRED)

set(CPACK_GENERATOR External NSIS)
set(CPACK_EXTERNAL_ENABLE_STAGING ON)
set(CPACK_EXTERNAL_PACKAGE_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/CMake/PostStagingScript.cmake")

include(CPack)
