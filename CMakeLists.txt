# Copyright Take Vos 2021.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.17)
cmake_policy(SET CMP0074 NEW)
enable_testing()
set(CMAKE_USER_MAKE_RULES_OVERRIDE "CMakeOverride.txt")

project(ttauri_hello_world)

add_subdirectory(ttauri)

if(WIN32)
  set(GUI_TYPE WIN32)
elseif(APPLE)
  set(GUI_TYPE MACOSX_BUNDLE)
endif()

add_executable(hello_world ${GUI_TYPE})
target_link_libraries(hello_world PRIVATE ttauri)
target_sources(hello_world PRIVATE src/main.cpp)

# Copy library resource files to application.
add_custom_target(hello_world_resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/ttauri/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
  DEPENDS
    resources/locale/en.po
    resources/locale/nl.po
    resources/hello_world.png
  )

add_dependencies(hello_world hello_world_resources)