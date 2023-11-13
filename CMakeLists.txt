# -*- mode: cmake-ts -*-
# https://github.com/raysan5/raylib/blob/535680668be3ca2e688a73132150c16f0160e0cc/projects/CMake/CMakeLists.txt

cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+

project(noterr LANGUAGES CXX)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 4.5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

# Our Project

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
  source/main.cxx
  source/ui.cxx
  source/popup.cxx
  source/raylib_helper.cxx
  source/note.cxx
  source/single_line_input.cxx
  source/multi_line_input.cxx
  source/todo_list.cxx
  source/stb_implementation.cxx)
set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)

if(MSVC)
else()
  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
  endif()

  if(CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_options(${PROJECT_NAME} PRIVATE -O2)
  endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL Release)
  target_compile_definitions(${PROJECT_NAME} PRIVATE NO_RAYLIB_INFO)
endif()

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wconversion)
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework IOKit")
  target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
  target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
