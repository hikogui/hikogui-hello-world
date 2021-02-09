
cmake_minimum_required(VERSION 3.17)
cmake_policy(SET CMP0074 NEW)
enable_testing()

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
add_custom_command(
  TARGET hello_world POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_BINARY_DIR}/data)
