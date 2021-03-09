# Copyright Take Vos 2021.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.19)

#-------------------------------------------------------------------
# Configure early CMAKE paths
#-------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

# Configure Vcpkg, including setting the `CMAKE_TOOLCHAIN_FILE` from the
# `VCPKG_ROOT` environment variable.
include(SetupVcpkg)

# Configure the compiler better for windows development,
# such that ReleaseWithDebug build produce the same assembly code as
# the Release build.
include(SetupCompiler)

#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------

# vcpkg.json is the primary source for version data
file(READ ${CMAKE_SOURCE_DIR}/vcpkg.json VCPKG_JSON_STRING)
string(JSON APP_NAME GET ${VCPKG_JSON_STRING} "name")
string(JSON APP_VERSION GET ${VCPKG_JSON_STRING} "version-string")
string(JSON APP_LICENSE GET ${VCPKG_JSON_STRING} "license")
string(JSON APP_DESCRIPTION GET ${VCPKG_JSON_STRING} "description")
string(JSON APP_HOMEPAGE GET ${VCPKG_JSON_STRING} "homepage")
set(APP_VENDOR "Yoyodyne Propulsion Systems")

project(${APP_NAME} VERSION ${APP_VERSION} LANGUAGES CXX)

#-------------------------------------------------------------------
# Find Dependencies
#-------------------------------------------------------------------
# The ttauri library and its dependencies are installed by vcpkg
# due to this `find_package()` command. The normal `find_package()`
# command was replaced by the vcpkg.cmake file, passed by the
# `CMAKE_TOOLCHAIN_FILE` variable. The `CMAKE_TOOLCHAIN_FILE` was
# set by the include(SetupVcpkg) earlier in this file.
find_package(ttauri REQUIRED)

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
configure_file("src/current_version.hpp.in" "src/current_version.hpp" @ONLY)
target_include_directories(hello_world PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/src")

target_sources(hello_world PRIVATE "src/main.cpp" "src/current_version.hpp")

# The resources needed by the ttauri library, and the resources needed for this application
# are both copied into the same resources directory next to the executable.
add_custom_target(hello_world_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ttauri_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources"
    DEPENDS
        "resources/locale/en.po"
        "resources/locale/nl.po"
        "resources/hello_world.png"
)
add_dependencies(hello_world hello_world_resources)

#-------------------------------------------------------------------
# Installation Rules: hello_world
#-------------------------------------------------------------------

install(TARGETS hello_world DESTINATION ".")

# Copy the resources from ttauri and from the hello world application
# to the install directory.
install(DIRECTORY "${ttauri_DIR}/resources/" DESTINATION "resources")
install(DIRECTORY "resources/" DESTINATION "resources")

# copy vcpkg dependencies (experimental)
x_vcpkg_install_local_dependencies(TARGETS hello_world)

#-------------------------------------------------------------------
# Packaging Rules: hello_world
#-------------------------------------------------------------------
if(WIN32)
    set(CPACK_GENERATOR NSIS)
endif()

set(CPACK_PACKAGE_VENDOR "Yoyodyne Propulsion Systems")
set(CPACK_PACKAGE_DESCRIPTION "${APP_DESCRIPTION}")
set(CPACK_PACKAGE_HOMEPAGE_URL "${APP_HOMEPAGE}")
set(CPACK_SYSTEM_NAME "${VCPKG_TARGET_TRIPLET}")
set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\CPack\\\\header.bmp")
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\CPack\\\\icon.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\CPack\\\\icon.ico")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}\\\\CPack\\\\license.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}\\\\CPack\\\\readme.txt")
set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_SOURCE_DIR}\\\\CPack\\\\welcome.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}\\\\CPack\\\\license.txt")

# Install VC_redist.x64.exe silently on the user's system, then remove the installer.
install(PROGRAMS "${CMAKE_CURRENT_SOURCE_DIR}/CPack/VC_redist.x64.exe" DESTINATION ".")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\VC_redist.x64.exe\\\" /q'")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "Delete '$INSTDIR\\\\VC_redist.x64.exe'")
string(REPLACE ";" "\n" CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}")

include(CPack)


