# Makefile for dimacs2graphs project
# Builds CLI, API, Backbone Solver, and example programs

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

# Compiler-specific flags
ifeq ($(COMPILER),gcc)
    CXXFLAGS = -std=c++17 -O3 -march=native -flto=auto -Wall -Wextra
    LDFLAGS = -flto=auto -fuse-linker-plugin
else ifeq ($(COMPILER),clang)
    CXXFLAGS = -std=c++17 -O3 -march=native -flto=thin -Wall -Wextra
    LDFLAGS = -flto=thin
endif

# Directories
BACKBONE_SOLVER_DIR = backbone_solver/src
BACKBONE_SOLVER_BIN = backbone_solver/bin
CLI_DIR = cli
API_DIR = api
BIN_DIR = bin

# BoneDigger paths
BACKBONE_SOLVER_API_OBJ = $(BACKBONE_SOLVER_DIR)/api/BackboneSolverAPI.o
BACKBONE_SOLVER_DETECTOR_OBJS = $(BACKBONE_SOLVER_DIR)/detectors/CheckCandidatesOneByOne.o \
                           $(BACKBONE_SOLVER_DIR)/detectors/CheckCandidatesOneByOneWithoutAttention.o
BACKBONE_SOLVER_IO_OBJ = $(BACKBONE_SOLVER_DIR)/io/DIMACSReader.o
BACKBONE_SOLVER_DATA_STRUCTURES_OBJ = $(BACKBONE_SOLVER_DIR)/data_structures/LiteralSet.o
BACKBONE_SOLVER_MINISAT_INTERFACE_OBJ = $(BACKBONE_SOLVER_DIR)/minisat_interface/minisat_aux.o
BACKBONE_SOLVER_MINISAT_LIB = $(BACKBONE_SOLVER_DIR)/minisat/build/release/lib/libminisat.a

BACKBONE_SOLVER_OBJS = $(BACKBONE_SOLVER_API_OBJ) \
                  $(BACKBONE_SOLVER_DETECTOR_OBJS) \
                  $(BACKBONE_SOLVER_IO_OBJ) \
                  $(BACKBONE_SOLVER_DATA_STRUCTURES_OBJ) \
                  $(BACKBONE_SOLVER_MINISAT_INTERFACE_OBJ)

# Targets
CLI_TARGET = $(BIN_DIR)/dimacs2graphs
API_EXAMPLE_TARGET = $(BIN_DIR)/api_example
BACKBONE_SOLVER_CLI_TARGET = $(BACKBONE_SOLVER_BIN)/backbone_solver

# Source files
CLI_SRC = $(CLI_DIR)/dimacs2graphs.cc
API_SRC = $(API_DIR)/Dimacs2GraphsAPI.cc
API_EXAMPLE_SRC = $(API_DIR)/api_example.cc

# Object files
API_OBJ = $(API_DIR)/Dimacs2GraphsAPI.o
CLI_OBJ = $(CLI_DIR)/dimacs2graphs.o
API_EXAMPLE_OBJ = $(API_DIR)/api_example.o

# Detect OS for linking
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LDLIBS = -lz -pthread
else ifeq ($(UNAME_S),Darwin)
    LDLIBS = -lz -pthread
else
    LDLIBS = -lz -pthread
endif

# Default target
.PHONY: all
all: $(BIN_DIR) cli api_example

# Create bin directory
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Build dimacs2graphs CLI
.PHONY: cli
cli: $(CLI_TARGET)

$(CLI_TARGET): $(CLI_OBJ) $(API_OBJ) $(BACKBONE_SOLVER_OBJS) $(BACKBONE_SOLVER_MINISAT_LIB) | $(BIN_DIR)
	@echo "Linking CLI: $@"
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	@echo "CLI built successfully: $@"

$(CLI_OBJ): $(CLI_SRC) $(API_DIR)/Dimacs2GraphsAPI.hh
	@echo "Compiling CLI: $<"
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Build API example
.PHONY: api_example
api_example: $(API_EXAMPLE_TARGET)

$(API_EXAMPLE_TARGET): $(API_EXAMPLE_OBJ) $(API_OBJ) $(BACKBONE_SOLVER_OBJS) $(BACKBONE_SOLVER_MINISAT_LIB) | $(BIN_DIR)
	@echo "Linking API example: $@"
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	@echo "API example built successfully: $@"

$(API_EXAMPLE_OBJ): $(API_EXAMPLE_SRC) $(API_DIR)/Dimacs2GraphsAPI.hh
	@echo "Compiling API example: $<"
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Build API object
$(API_OBJ): $(API_SRC) $(API_DIR)/Dimacs2GraphsAPI.hh
	@echo "Compiling API: $<"
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean targets
.PHONY: clean
clean:
	@echo "Cleaning project..."
	@rm -f $(CLI_OBJ) $(API_OBJ) $(API_EXAMPLE_OBJ)
	@rm -f $(CLI_TARGET) $(API_EXAMPLE_TARGET)
	@$(MAKE) -C $(BACKBONE_SOLVER_DIR) clean
	@echo "Clean complete"

.PHONY: distclean
distclean: clean
	@echo "Deep cleaning (including MiniSat)..."
	@$(MAKE) -C $(BACKBONE_SOLVER_DIR) distclean
	@rm -rf $(BIN_DIR)
	@echo "Deep clean complete"

# Help target
.PHONY: help
help:
	@echo "Dimacs2Graphs Makefile"
	@echo "======================"
	@echo ""
	@echo "Targets:"
	@echo "  all         - Build everything (default)"
	@echo "  cli         - Build dimacs2graphs CLI tool"
	@echo "  api_example - Build API usage example"
	@echo "  clean       - Remove build artifacts"
	@echo "  distclean   - Remove all build artifacts including MiniSat"
	@echo "  help        - Show this help message"
	@echo ""
	@echo "Compiler options:"
	@echo "  CXX=g++     - Use g++ compiler (default)"
	@echo "  CXX=clang++ - Use clang++ compiler"
	@echo ""
	@echo "Examples:"
	@echo "  make              - Build all targets"
	@echo "  make cli          - Build only the CLI"
	@echo "  make CXX=clang++  - Build with clang++"
	@echo "  make clean        - Clean build artifacts"

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

# Build MiniSat library
$(BACKBONE_SOLVER_MINISAT_LIB):
	@echo "Building MiniSat library..."
	@$(MAKE) -C $(BACKBONE_SOLVER_DIR) CXX=$(CXX) minisat/build/release/lib/libminisat.a

# Build individual Backbone Solver object files
$(BACKBONE_SOLVER_API_OBJ): $(BACKBONE_SOLVER_MINISAT_LIB)
	@echo "Building $@..."
	@$(MAKE) -C $(BACKBONE_SOLVER_DIR) CXX=$(CXX) api/BackboneSolverAPI.o

$(BACKBONE_SOLVER_DIR)/detectors/CheckCandidatesOneByOne.o: $(BACKBONE_SOLVER_MINISAT_LIB)
	@echo "Building $@..."
	@$(MAKE) -C $(BACKBONE_SOLVER_DIR) CXX=$(CXX) detectors/CheckCandidatesOneByOne.o

$(BACKBONE_SOLVER_DIR)/detectors/CheckCandidatesOneByOneWithoutAttention.o: $(BACKBONE_SOLVER_MINISAT_LIB)
	@echo "Building $@..."
	@$(MAKE) -C $(BACKBONE_SOLVER_DIR) CXX=$(CXX) detectors/CheckCandidatesOneByOneWithoutAttention.o

$(BACKBONE_SOLVER_IO_OBJ): $(BACKBONE_SOLVER_MINISAT_LIB)
	@echo "Building $@..."
	@$(MAKE) -C $(BACKBONE_SOLVER_DIR) CXX=$(CXX) io/DIMACSReader.o

$(BACKBONE_SOLVER_DATA_STRUCTURES_OBJ): $(BACKBONE_SOLVER_MINISAT_LIB)
	@echo "Building $@..."
	@$(MAKE) -C $(BACKBONE_SOLVER_DIR) CXX=$(CXX) data_structures/LiteralSet.o

$(BACKBONE_SOLVER_MINISAT_INTERFACE_OBJ): $(BACKBONE_SOLVER_MINISAT_LIB)
	@echo "Building $@..."
	@$(MAKE) -C $(BACKBONE_SOLVER_DIR) CXX=$(CXX) minisat_interface/minisat_aux.o
