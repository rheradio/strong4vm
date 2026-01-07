
PROGRAM_NAME := backbone_solver

#Source directories
CLI_DIR := cli
API_DIR := api
DETECTORS_DIR := detectors
CORE_DIR := data_structures
IO_DIR := io
MINISAT_INTERFACE_DIR := minisat_interface

#Source lists with proper paths
MAIN_SRCS := $(CLI_DIR)/main.cc $(API_DIR)/BackboneSolverAPI.cc $(IO_DIR)/DIMACSReader.cc \
             $(DETECTORS_DIR)/CheckCandidatesOneByOne.cc $(DETECTORS_DIR)/CheckCandidatesOneByOneWithoutAttention.cc \
             $(CORE_DIR)/LiteralSet.cc $(MINISAT_INTERFACE_DIR)/minisat_aux.cc

API_EXAMPLE_SRCS := $(API_DIR)/api_example.cc $(API_DIR)/BackboneSolverAPI.cc $(IO_DIR)/DIMACSReader.cc \
                    $(DETECTORS_DIR)/CheckCandidatesOneByOne.cc $(DETECTORS_DIR)/CheckCandidatesOneByOneWithoutAttention.cc \
                    $(CORE_DIR)/LiteralSet.cc $(MINISAT_INTERFACE_DIR)/minisat_aux.cc

# dimacs2graph tool sources
DIMACS2GRAPH_SRCS := ../../dimacs2graph.cc $(API_DIR)/BackboneSolverAPI.cc $(IO_DIR)/DIMACSReader.cc \
                     $(DETECTORS_DIR)/CheckCandidatesOneByOne.cc $(DETECTORS_DIR)/CheckCandidatesOneByOneWithoutAttention.cc \
                     $(CORE_DIR)/LiteralSet.cc $(MINISAT_INTERFACE_DIR)/minisat_aux.cc

MAIN_OBJS := $(MAIN_SRCS:.cc=.o)
API_EXAMPLE_OBJS := $(API_EXAMPLE_SRCS:.cc=.o)
DIMACS2GRAPH_OBJS := $(DIMACS2GRAPH_SRCS:.cc=.o)
DEPENDS  := $(wildcard */*.d)

# Compiler
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
    # Default to gcc if detection fails
    COMPILER := gcc
endif

# MiniSat library location
MINISAT_DIR := ./minisat/build/release/lib
MINISAT_LIB := $(MINISAT_DIR)/libminisat.a
MINISAT_TARGET ?= lr

# Output directory
BIN_DIR := ../bin

# Base flags (common to both compilers)
CCFLAGS := -O3 -march=native -mtune=native -ffast-math -DNDEBUG
CCFLAGS += -funroll-loops -finline-functions -fomit-frame-pointer -fmerge-all-constants
CCFLAGS += -fno-signed-zeros -fno-trapping-math -fno-math-errno -ffinite-math-only
CCFLAGS += -fno-stack-protector -falign-functions=32 -falign-loops=32
CCFLAGS += -std=c++17 -Wall -Wextra -pedantic -MMD -pipe
CCFLAGS += -I. -I$(CLI_DIR) -I$(API_DIR) -I$(DETECTORS_DIR) -I$(CORE_DIR) -I$(IO_DIR) -I$(MINISAT_INTERFACE_DIR) -I./minisat -DEXPERT

# Compiler-specific flags
ifeq ($(COMPILER),gcc)
    # GCC-specific optimizations
    CCFLAGS += -flto=auto -ftree-vectorize -fvect-cost-model=unlimited -ftree-loop-if-convert-stores
    CCFLAGS += -funswitch-loops -ftree-loop-distribution -fsplit-loops -fpeel-loops
    CCFLAGS += -fweb -frename-registers
    CCFLAGS += -fpredictive-commoning -fgcse-after-reload -fipa-pta
    CCFLAGS += -fdevirtualize-at-ltrans -fmodulo-sched -fmodulo-sched-allow-regmoves
    CCFLAGS += -ftree-partial-pre -fivopts -fgcse-sm -fgcse-las
    CCFLAGS += -ftracer -fbranch-target-load-optimize2 -fno-plt
    CCFLAGS += -fno-semantic-interposition -fipa-icf -fipa-icf-functions -fipa-icf-variables

    # GCC-specific linker flags
    LNFLAGS := -flto=auto -fuse-linker-plugin -Wl,-O3

    # Platform-specific linker flags for GCC
    ifeq ($(shell uname),Darwin)
        # macOS linker flags
        LNFLAGS += -Wl,-dead_strip
    else
        # Linux linker flags
        LNFLAGS += -Wl,--as-needed -Wl,--sort-common -Wl,--gc-sections
        LNFLAGS += -Wl,--hash-style=gnu -Wl,--build-id=none
        # Static linking on Linux
        LNFLAGS += -static
    endif
else ifeq ($(COMPILER),clang)
    # Clang-specific optimizations
    CCFLAGS += -flto=thin -fvectorize -fslp-vectorize
    CCFLAGS += -fstrict-vtable-pointers

    # Clang-specific linker flags
    LNFLAGS := -flto=thin -Wl,-O3

    # Platform-specific linker flags for clang
    ifeq ($(shell uname),Darwin)
        # macOS linker flags
        LNFLAGS += -Wl,-dead_strip
    else
        # Linux linker flags
        LNFLAGS += -Wl,--gc-sections -Wl,--as-needed
    endif
endif

LIBS := -L$(MINISAT_DIR) -lminisat -lz

.PHONY: all clean api api_example dimacs2graph distclean docs show-compiler test
all: $(BIN_DIR)/$(PROGRAM_NAME)

# Show detected compiler and its flags
show-compiler:
	@echo "Detected compiler: $(COMPILER)"
	@echo "CXX: $(CXX)"
	@echo "Compiler version:"
	@$(CXX) --version | head -n 1
	@echo ""
	@echo "CCFLAGS: $(CCFLAGS)"
	@echo ""
	@echo "LNFLAGS: $(LNFLAGS)"

# Create bin directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Ensure MiniSat library is available; build it when missing
$(MINISAT_LIB):
	@echo "Building MiniSat (target: $(MINISAT_TARGET))..."
ifeq ($(COMPILER),gcc)
	@cd minisat && $(MAKE) CXX=$(CXX) COPTIMIZE="-O3 -flto=auto -fno-strict-aliasing -DNDEBUG" $(MINISAT_TARGET)
else
	@cd minisat && $(MAKE) CXX=$(CXX) $(MINISAT_TARGET)
endif

# Main program target (only links main objects)
$(BIN_DIR)/$(PROGRAM_NAME): $(MAIN_OBJS) $(MINISAT_LIB) | $(BIN_DIR)
	@echo Linking $@
	$(CXX) $(LNFLAGS) -o $@ $(MAIN_OBJS) $(LIBS)

# API example target (separate executable)
api: api_example

api_example: $(BIN_DIR)/api_example

$(BIN_DIR)/api_example: $(API_EXAMPLE_OBJS) $(MINISAT_LIB) | $(BIN_DIR)
	@echo Linking $@
	$(CXX) $(LNFLAGS) -o $@ $(API_EXAMPLE_OBJS) $(LIBS)

# dimacs2graph target
dimacs2graph: $(BIN_DIR)/dimacs2graph

$(BIN_DIR)/dimacs2graph: $(DIMACS2GRAPH_OBJS) $(MINISAT_LIB) | $(BIN_DIR)
	@echo Linking $@
	$(CXX) $(LNFLAGS) -o $@ $(DIMACS2GRAPH_OBJS) $(LIBS)

# Test target
test: dimacs2graph
	@echo "Running dimacs2graph test suite..."
	@cd ../../test && bash run_tests.sh

# Pattern rule for compiling .cc -> .o
%.o: %.cc
	@echo Compiling $@
	$(CXX) $(CCFLAGS) -c -o $@ $<

clean:
	@echo Cleaning build artifacts...
	@rm -f $(CLI_DIR)/*.o $(API_DIR)/*.o $(DETECTORS_DIR)/*.o $(CORE_DIR)/*.o $(IO_DIR)/*.o $(MINISAT_INTERFACE_DIR)/*.o
	@rm -f $(CLI_DIR)/*.d $(API_DIR)/*.d $(DETECTORS_DIR)/*.d $(CORE_DIR)/*.d $(IO_DIR)/*.d $(MINISAT_INTERFACE_DIR)/*.d
	@rm -f ../../dimacs2graph.o ../../dimacs2graph.d
	@rm -f $(BIN_DIR)/$(PROGRAM_NAME) $(BIN_DIR)/api_example $(BIN_DIR)/dimacs2graph
	@rm -rf $(BIN_DIR)

distclean: clean
	@cd minisat && $(MAKE) clean || true

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
	@mkdir -p ../docs
	@cd ../docs && doxygen Doxyfile
	@echo ""
	@echo "Documentation generated in ../docs/html/"
	@echo "Open ../docs/html/index.html in your browser"

-include $(DEPENDS)