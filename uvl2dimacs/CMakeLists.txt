cmake_minimum_required(VERSION 3.10)
project(uvl2dimacs VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for binaries - place directly in build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add CMAKE_PREFIX_PATH for finding ANTLR4 runtime
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local")

# Check if ANTLR4 paths are provided via command line
if(DEFINED ANTLR4_INCLUDE_DIR AND DEFINED ANTLR4_LIBRARY)
    # Use provided ANTLR4 runtime
    message(STATUS "Using provided ANTLR4 runtime: ${ANTLR4_LIBRARY}")
    message(STATUS "Using provided ANTLR4 headers: ${ANTLR4_INCLUDE_DIR}")
    set(ANTLR4_INCLUDE_DIRS ${ANTLR4_INCLUDE_DIR})
    # Create an imported target for antlr4-runtime
    add_library(antlr4-runtime STATIC IMPORTED)
    set_target_properties(antlr4-runtime PROPERTIES
        IMPORTED_LOCATION ${ANTLR4_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${ANTLR4_INCLUDE_DIR}
    )
else()
    # Find ANTLR4 runtime
    find_package(antlr4-runtime REQUIRED)
endif()

# Build parser inline (avoid separate parser/build/ directory)
# Collect all generated parser source files
file(GLOB PARSER_SOURCES ${PROJECT_SOURCE_DIR}/parser/src/*.cpp)

# Create parser library
add_library(uvl-parser STATIC ${PARSER_SOURCES})
target_include_directories(uvl-parser PUBLIC
    ${PROJECT_SOURCE_DIR}/parser/include
    ${ANTLR4_INCLUDE_DIRS}
)
target_link_libraries(uvl-parser antlr4-runtime)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/generator/include
    ${PROJECT_SOURCE_DIR}/parser/include
    ${ANTLR4_INCLUDE_DIRS}
)

# Source files for the library
set(LIB_SOURCES
    generator/src/ASTNode.cc
    generator/src/Constraint.cc
    generator/src/Relation.cc
    generator/src/Feature.cc
    generator/src/FeatureModel.cc
    generator/src/CNFModel.cc
    generator/src/RelationEncoder.cc
    generator/src/FMToCNF.cc
    generator/src/DimacsWriter.cc
    generator/src/FeatureModelBuilder.cc
)

# Create library
add_library(uvl2dimacs_lib STATIC ${LIB_SOURCES})

# Link ANTLR4 runtime and UVL parser to library
target_link_libraries(uvl2dimacs_lib uvl-parser antlr4-runtime)

# Main executable (CLI)
add_executable(uvl2dimacs cli/uvl2dimacs.cc)
target_link_libraries(uvl2dimacs uvl2dimacs_lib uvl-parser antlr4-runtime)

# Enable testing
enable_testing()

# Test executable (will be created later)
# add_executable(test_uvl2dimacs tests/test_main.cpp)
# target_link_libraries(test_uvl2dimacs uvl2dimacs_lib antlr4-runtime)
# add_test(NAME test_uvl2dimacs COMMAND test_uvl2dimacs)

# API Library
set(API_SOURCES
    api/src/UVL2Dimacs.cc
)

add_library(uvl2dimacs_api STATIC ${API_SOURCES})
target_include_directories(uvl2dimacs_api PUBLIC
    ${PROJECT_SOURCE_DIR}/api/include
    ${PROJECT_SOURCE_DIR}/generator/include
    ${PROJECT_SOURCE_DIR}/parser/include
    ${ANTLR4_INCLUDE_DIRS}
)
target_link_libraries(uvl2dimacs_api uvl2dimacs_lib uvl-parser antlr4-runtime)

# Installation
install(TARGETS uvl2dimacs_lib uvl2dimacs_api uvl2dimacs
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY generator/include/
    DESTINATION include/uvl2dimacs
)

install(DIRECTORY api/include/
    DESTINATION include
)
