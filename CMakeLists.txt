cmake_minimum_required(VERSION 3.30)
project(palladium VERSION 0.0.1)

SET(CMAKE_CXX_STANDARD 26)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose build type (Debug, Release.)" FORCE)
endif()

add_compile_options(
  -Wall
  -Wextra
  -Wpedantic
)


if( CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
      -Wshadow
      -Wnon-virtual-dtor
      -Wold-style-cast
      -Wcast-align
      -Woverloaded-virtual
      -Wnull-dereference
      -Wdouble-promotion
      -Wformat=2
      -fsanitize=address
      -g
    )
    link_libraries(-fsanitize=address)
  endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_options(
    -O3
  )
endif()

add_executable("${CMAKE_PROJECT_NAME}" $<TARGET_OBJECTS:OBJECT_LIB>)

add_subdirectory(src)
enable_testing()
add_subdirectory(tests)


