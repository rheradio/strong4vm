cmake_minimum_required(VERSION 3.10)
project(uvl-parser-cpp VERSION 0.4.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ANTLR4 runtime
# First check if ANTLR4 paths were provided via command-line
if(DEFINED ANTLR4_INCLUDE_DIR AND DEFINED ANTLR4_LIBRARY)
  set(ANTLR4_INCLUDE_DIRS ${ANTLR4_INCLUDE_DIR})
  set(ANTLR4_LIBRARIES ${ANTLR4_LIBRARY})
  set(ANTLR4_FOUND TRUE)
  message(STATUS "Using provided ANTLR4 runtime: ${ANTLR4_LIBRARIES}")
  message(STATUS "Using provided ANTLR4 headers: ${ANTLR4_INCLUDE_DIRS}")
endif()

# Try multiple methods to locate ANTLR4 runtime if not provided
if(NOT ANTLR4_FOUND)
  find_package(PkgConfig)
  if(PKG_CONFIG_FOUND)
    pkg_check_modules(ANTLR4 antlr4-runtime)
  endif()
endif()

# If pkg-config didn't find it, search in standard locations
if(NOT ANTLR4_FOUND)
  find_path(ANTLR4_INCLUDE_DIRS antlr4-runtime.h
    PATHS
      /usr/local/include/antlr4-runtime
      /usr/include/antlr4-runtime
      /tmp/antlr4/runtime/Cpp/runtime/src
  )
  find_library(ANTLR4_LIBRARIES
    NAMES antlr4-runtime
    PATHS
      /usr/local/lib
      /usr/lib
      /tmp/antlr4/runtime/Cpp/build/runtime
  )
  if(ANTLR4_INCLUDE_DIRS AND ANTLR4_LIBRARIES)
    set(ANTLR4_FOUND TRUE)
    message(STATUS "Found ANTLR4 runtime: ${ANTLR4_LIBRARIES}")
    message(STATUS "Found ANTLR4 headers: ${ANTLR4_INCLUDE_DIRS}")
  endif()
endif()

if(NOT ANTLR4_FOUND)
  message(FATAL_ERROR "ANTLR4 runtime not found. Please install it first.")
endif()

# Include directories
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${ANTLR4_INCLUDE_DIRS}
)

# Collect all generated parser source files
file(GLOB PARSER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Create library
add_library(uvl-parser STATIC ${PARSER_SOURCES})

target_link_libraries(uvl-parser
  ${ANTLR4_LIBRARIES}
)

# Install targets
install(TARGETS uvl-parser
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION include/uvl-parser
  FILES_MATCHING PATTERN "*.h"
)
