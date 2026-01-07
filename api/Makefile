# Makefile for Strong4VM API and Examples
#
# Builds the Strong4VM C++ API library and example programs

# Compiler settings
CXX ?= g++

# Detect compiler type
COMPILER_VERSION := $(shell $(CXX) --version 2>/dev/null)
ifneq (,$(findstring clang,$(COMPILER_VERSION)))
    COMPILER := clang
else ifneq (,$(findstring g++,$(COMPILER_VERSION)))
    COMPILER := gcc
else ifneq (,$(findstring GCC,$(COMPILER_VERSION)))
    COMPILER := gcc
else
    COMPILER := gcc
endif

CXXFLAGS := -std=c++17 -Wall -Wextra -O3 -march=native
INCLUDES := -Iinclude \
            -I../uvl2dimacs/api/include \
            -I../dimacs2graphs/api

# UVL2DIMACS dependencies
UVL2DIMACS_BUILD := ../uvl2dimacs/build
UVL2DIMACS_LIBS := $(UVL2DIMACS_BUILD)/libuvl2dimacs_api.a \
                   $(UVL2DIMACS_BUILD)/libuvl2dimacs_lib.a \
                   $(UVL2DIMACS_BUILD)/libuvl-parser.a \
                   ../uvl2dimacs/third_party/build/runtime/libantlr4-runtime.a

# Dimacs2Graphs dependencies
DIMACS2GRAPHS_API := ../dimacs2graphs/api/Dimacs2GraphsAPI.o
BACKBONE_SOLVER_DIR := ../dimacs2graphs/backbone_solver/src
BACKBONE_SOLVER_OBJS := $(BACKBONE_SOLVER_DIR)/api/BackboneSolverAPI.o \
                   $(BACKBONE_SOLVER_DIR)/detectors/CheckCandidatesOneByOne.o \
                   $(BACKBONE_SOLVER_DIR)/detectors/CheckCandidatesOneByOneWithoutAttention.o \
                   $(BACKBONE_SOLVER_DIR)/io/DIMACSReader.o \
                   $(BACKBONE_SOLVER_DIR)/data_structures/LiteralSet.o \
                   $(BACKBONE_SOLVER_DIR)/minisat_interface/minisat_aux.o
MINISAT_LIB := $(BACKBONE_SOLVER_DIR)/minisat/build/release/lib/libminisat.a

# All external libraries
EXT_LIBS := $(UVL2DIMACS_LIBS) $(DIMACS2GRAPHS_API) $(BACKBONE_SOLVER_OBJS) $(MINISAT_LIB)

# Detect OS and compiler version for system libraries
UNAME_S := $(shell uname -s)

# Check if we need -lstdc++fs (only for GCC < 9.1)
NEED_STDCXXFS := 0
ifeq ($(COMPILER),gcc)
    GCC_VERSION := $(shell $(CXX) -dumpversion | cut -d. -f1)
    ifeq ($(shell test $(GCC_VERSION) -lt 9 && echo 1),1)
        NEED_STDCXXFS := 1
    endif
endif

# Platform-specific linking flags
ifeq ($(UNAME_S),Linux)
    SYS_LIBS := -lz -pthread -ldl
    ifeq ($(NEED_STDCXXFS),1)
        SYS_LIBS += -lstdc++fs
    endif
else ifeq ($(UNAME_S),Darwin)
    SYS_LIBS := -lz -pthread
else
    # Default for other Unix-like systems
    SYS_LIBS := -lz -pthread
    ifeq ($(NEED_STDCXXFS),1)
        SYS_LIBS += -lstdc++fs -ldl
    endif
endif

# Directories
SRC_DIR := src
INCLUDE_DIR := include
EXAMPLES_DIR := examples
BUILD_DIR := build
LIB_DIR := lib
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := ../bin
EXAMPLES_BUILD_DIR := $(BIN_DIR)

# Source files
API_SOURCES := $(wildcard $(SRC_DIR)/*.cc)
API_OBJECTS := $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(API_SOURCES))

# Library
API_LIBRARY := $(LIB_DIR)/libstrong4vm_api.a

# Example sources
EXAMPLE_SOURCES := $(wildcard $(EXAMPLES_DIR)/*.cc)
EXAMPLE_TARGETS := $(patsubst $(EXAMPLES_DIR)/%.cc,$(EXAMPLES_BUILD_DIR)/%,$(EXAMPLE_SOURCES))

# Targets
.PHONY: all api examples clean help install

all: api examples

# Build API library
api: $(API_LIBRARY)

$(API_LIBRARY): $(API_OBJECTS) | $(LIB_DIR)
	@echo "Creating static library: $@"
	ar rcs $@ $^
	@echo "✓ API library built successfully"

# Compile API object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Build all examples
examples: $(EXAMPLE_TARGETS)

# Build individual examples
$(EXAMPLES_BUILD_DIR)/%: $(EXAMPLES_DIR)/%.cc $(API_LIBRARY) | $(EXAMPLES_BUILD_DIR)
	@echo "Building example: $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@ \
		-L$(LIB_DIR) -lstrong4vm_api \
		$(EXT_LIBS) $(SYS_LIBS)
	@echo "✓ Example built: $(notdir $@)"

# Convenience targets for individual examples
.PHONY: simple advanced batch

simple: $(EXAMPLES_BUILD_DIR)/simple_analysis

advanced: $(EXAMPLES_BUILD_DIR)/advanced_analysis

batch: $(EXAMPLES_BUILD_DIR)/batch_analysis

# Create directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

$(EXAMPLES_BUILD_DIR):
	@mkdir -p $(BIN_DIR)

# Install library to ~/bin (optional)
install: api
	@echo "Installing Strong4VM API..."
	@mkdir -p ~/include/strong4vm
	@mkdir -p ~/lib
	@cp -r $(INCLUDE_DIR)/strong4vm/*.hh ~/include/strong4vm/
	@cp $(API_LIBRARY) ~/lib/
	@echo "✓ API installed to ~/include and ~/lib"

# Clean build artifacts
clean:
	@echo "Cleaning API build artifacts..."
	@rm -rf $(BUILD_DIR) $(LIB_DIR)
	@echo "✓ Clean complete"

# Help target
help:
	@echo "Strong4VM API Makefile"
	@echo "====================="
	@echo ""
	@echo "Targets:"
	@echo "  make all        - Build API library and all examples (default)"
	@echo "  make api        - Build only the API library"
	@echo "  make examples   - Build all example programs"
	@echo "  make simple     - Build simple_analysis example"
	@echo "  make advanced   - Build advanced_analysis example"
	@echo "  make batch      - Build batch_analysis example"
	@echo "  make install    - Install API to ~/include and ~/lib"
	@echo "  make clean      - Remove all build artifacts"
	@echo "  make help       - Show this help message"
	@echo ""
	@echo "Example Usage:"
	@echo "  make"
	@echo "  ./bin/simple_analysis model.uvl"
	@echo "  ./bin/advanced_analysis model.uvl ./output 4"
	@echo "  ./bin/batch_analysis ./models ./output 8"

# Build Doxygen documentation
.PHONY: docs
docs:
	@echo "========================================="
	@echo "Generating Documentation with Doxygen..."
	@echo "========================================="
	@command -v doxygen >/dev/null 2>&1 || { \
		echo "ERROR: doxygen not found. Install it:"; \
		echo "  Linux:  sudo apt-get install doxygen graphviz"; \
		echo "  macOS:  brew install doxygen graphviz"; \
		exit 1; \
	}
	@mkdir -p docs
	@cd docs && doxygen Doxyfile
	@echo ""
	@echo "Documentation generated in docs/html/"
	@echo "Open docs/html/index.html in your browser"
