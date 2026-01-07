# Makefile for Strong4VM - From Variability Models to Strong Transitive Dependency and Conflict Graphs
# Builds uvl2dimacs, dimacs2graphs, and the unified strong4vm CLI tool

# Compiler settings
CXX ?= g++

# Detect OS first (needed for compiler flag decisions)
UNAME_S := $(shell uname -s)

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
    # On macOS, avoid LTO in linking to prevent issues with static archives
    ifeq ($(UNAME_S),Darwin)
        CXXFLAGS = -std=c++17 -O3 -march=native -Wall -Wextra
        LDFLAGS =
    else
        CXXFLAGS = -std=c++17 -O3 -march=native -flto=thin -Wall -Wextra
        LDFLAGS = -flto=thin
    endif
endif

# Directories
UVL2DIMACS_DIR = uvl2dimacs
DIMACS2GRAPHS_DIR = dimacs2graphs
API_DIR = api
CLI_DIR = cli
BIN_DIR = bin

# Strong4VM paths
STRONG4VM_SRC = $(CLI_DIR)/strong4vm.cc
STRONG4VM_TARGET = $(BIN_DIR)/strong4vm
ICON_HEADER = $(CLI_DIR)/icon_embedded.hh
ICON_GENERATOR = $(CLI_DIR)/generate_icon_header.sh

# UVL2DIMACS dependencies
UVL2DIMACS_BUILD = $(UVL2DIMACS_DIR)/build
UVL2DIMACS_API_LIB = $(UVL2DIMACS_BUILD)/libuvl2dimacs_api.a
UVL2DIMACS_LIB = $(UVL2DIMACS_BUILD)/libuvl2dimacs_lib.a
UVL2DIMACS_PARSER_LIB = $(UVL2DIMACS_BUILD)/libuvl-parser.a
ANTLR4_LIB = $(UVL2DIMACS_DIR)/third_party/build/runtime/libantlr4-runtime.a

# Dimacs2Graphs dependencies
DIMACS2GRAPHS_API = $(DIMACS2GRAPHS_DIR)/api/Dimacs2GraphsAPI.o
BACKBONE_SOLVER_DIR = $(DIMACS2GRAPHS_DIR)/backbone_solver/src
BACKBONE_SOLVER_OBJS = $(BACKBONE_SOLVER_DIR)/api/BackboneSolverAPI.o \
                  $(BACKBONE_SOLVER_DIR)/detectors/CheckCandidatesOneByOne.o \
                  $(BACKBONE_SOLVER_DIR)/detectors/CheckCandidatesOneByOneWithoutAttention.o \
                  $(BACKBONE_SOLVER_DIR)/io/DIMACSReader.o \
                  $(BACKBONE_SOLVER_DIR)/data_structures/LiteralSet.o \
                  $(BACKBONE_SOLVER_DIR)/minisat_interface/minisat_aux.o
MINISAT_LIB = $(BACKBONE_SOLVER_DIR)/minisat/build/release/lib/libminisat.a

# Include paths
INCLUDES = -I$(UVL2DIMACS_DIR)/api/include \
           -I$(UVL2DIMACS_DIR)/generator/include \
           -I$(UVL2DIMACS_DIR)/parser/include \
           -I$(UVL2DIMACS_DIR)/third_party/runtime/src \
           -I$(DIMACS2GRAPHS_DIR)/api \
           -I$(DIMACS2GRAPHS_DIR)/backbone_solver/src

# Libraries
LIBS = $(UVL2DIMACS_API_LIB) \
       $(UVL2DIMACS_LIB) \
       $(UVL2DIMACS_PARSER_LIB) \
       $(ANTLR4_LIB) \
       $(DIMACS2GRAPHS_API) \
       $(BACKBONE_SOLVER_OBJS) \
       $(MINISAT_LIB)

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
    LDLIBS = -lz -pthread -ldl
    ifeq ($(NEED_STDCXXFS),1)
        LDLIBS += -lstdc++fs
    endif
else ifeq ($(UNAME_S),Darwin)
    LDLIBS = -lz -pthread
else
    # Default for other Unix-like systems
    LDLIBS = -lz -pthread
    ifeq ($(NEED_STDCXXFS),1)
        LDLIBS += -lstdc++fs -ldl
    endif
endif

# Default target
.PHONY: all
all: $(BIN_DIR) strong4vm

# Create bin directory
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Build strong4vm CLI
.PHONY: strong4vm
strong4vm: $(STRONG4VM_TARGET)

# Generate icon header from figures/icon.txt
$(ICON_HEADER): figures/icon.txt $(ICON_GENERATOR)
	@echo "Generating embedded icon header..."
	@# Fix line endings if needed (handles Windows CRLF -> Unix LF conversion)
	@if file $(ICON_GENERATOR) 2>/dev/null | grep -q CRLF 2>/dev/null || \
	   grep -q $$'\r' $(ICON_GENERATOR) 2>/dev/null; then \
		echo "Fixing line endings in $(ICON_GENERATOR)..."; \
		if [ "$(UNAME_S)" = "Darwin" ]; then \
			sed -i '' 's/\r$$//' $(ICON_GENERATOR); \
		else \
			sed -i 's/\r$$//' $(ICON_GENERATOR); \
		fi; \
	fi
	@chmod +x $(ICON_GENERATOR)
	@cd $(CLI_DIR) && bash generate_icon_header.sh

$(STRONG4VM_TARGET): $(STRONG4VM_SRC) $(ICON_HEADER) $(LIBS) | $(BIN_DIR)
	@echo "========================================="
	@echo "Building Strong4VM..."
	@echo "========================================="
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(LIBS) $(LDLIBS) $(LDFLAGS)
	@echo ""
	@echo "Strong4VM built successfully: $@"
	@echo ""
	@echo "Usage: $@ <input_file> [options]"
	@echo "Run '$@ --help' for more information"

# Build UVL2DIMACS
.PHONY: uvl2dimacs
uvl2dimacs: $(UVL2DIMACS_API_LIB)

$(UVL2DIMACS_API_LIB) $(UVL2DIMACS_LIB) $(UVL2DIMACS_PARSER_LIB) $(ANTLR4_LIB):
	@echo "Building UVL2DIMACS..."
	@$(MAKE) -C $(UVL2DIMACS_DIR) all

# Build Dimacs2Graphs
.PHONY: dimacs2graphs
dimacs2graphs: $(DIMACS2GRAPHS_API)

$(DIMACS2GRAPHS_API): $(BACKBONE_SOLVER_OBJS)
	@echo "Building Dimacs2Graphs API..."
	@$(MAKE) -C $(DIMACS2GRAPHS_DIR) cli

$(BACKBONE_SOLVER_OBJS) $(MINISAT_LIB):
	@echo "Building Backbone Solver..."
	@$(MAKE) -C $(BACKBONE_SOLVER_DIR) CXX=$(CXX)

# Build API
.PHONY: api
api: uvl2dimacs dimacs2graphs
	@echo "Building Strong4VM API..."
	@$(MAKE) -C $(API_DIR) api

# Build API examples
.PHONY: api-examples
api-examples: api
	@echo "Building Strong4VM API examples..."
	@$(MAKE) -C $(API_DIR) examples

# Build all components
.PHONY: components
components: uvl2dimacs dimacs2graphs

# Clean targets
.PHONY: clean
clean:
	@echo "Cleaning Strong4VM..."
	@rm -f $(STRONG4VM_TARGET)
	@rm -f $(ICON_HEADER)
	@echo "Cleaning API..."
	@$(MAKE) -C $(API_DIR) clean
	@echo "Cleaning Dimacs2Graphs..."
	@$(MAKE) -C $(DIMACS2GRAPHS_DIR) clean
	@echo "Clean complete"

.PHONY: clean-all
clean-all: clean
	@echo "Deep cleaning UVL2DIMACS..."
	@$(MAKE) -C $(UVL2DIMACS_DIR) clean-all
	@echo "Deep cleaning Dimacs2Graphs..."
	@$(MAKE) -C $(DIMACS2GRAPHS_DIR) distclean
	@rm -rf $(BIN_DIR)
	@echo "Deep clean complete"

# Install
.PHONY: install
install: strong4vm
	@echo "Installing strong4vm CLI..."
	@mkdir -p $(HOME)/bin
	@cp $(STRONG4VM_TARGET) $(HOME)/bin/
	@echo "Installed to $(HOME)/bin/strong4vm"
	@echo "Make sure $(HOME)/bin is in your PATH"

# Install API
.PHONY: install-api
install-api: api
	@echo "Installing Strong4VM API..."
	@$(MAKE) -C $(API_DIR) install
	@echo "API installed to ~/include and ~/lib"

# Help target
.PHONY: help
help:
	@echo "==========================================================================="
	@echo "Strong4VM:"
	@echo "From Variability Models to Strong Transitive Dependency and Conflict Graphs"
	@echo "==========================================================================="
	@echo ""
	@echo "Targets:"
	@echo "  all            - Build strong4vm CLI (default)"
	@echo "  strong4vm      - Build strong4vm CLI"
	@echo "  api            - Build Strong4VM API library"
	@echo "  api-examples   - Build API example programs"
	@echo "  components     - Build uvl2dimacs and dimacs2graphs separately"
	@echo "  uvl2dimacs     - Build uvl2dimacs component"
	@echo "  dimacs2graphs  - Build dimacs2graphs component"
	@echo "  clean          - Clean build artifacts"
	@echo "  clean-all      - Deep clean (including all components)"
	@echo "  install        - Install strong4vm CLI to ~/bin"
	@echo "  install-api    - Install API to ~/include and ~/lib"
	@echo "  help           - Show this help message"
	@echo ""
	@echo "Compiler options:"
	@echo "  CXX=g++        - Use g++ compiler (default)"
	@echo "  CXX=clang++    - Use clang++ compiler"
	@echo ""
	@echo "Examples:"
	@echo "  make                    - Build strong4vm"
	@echo "  make CXX=clang++        - Build with clang++"
	@echo "  make clean all          - Rebuild from scratch"
	@echo "  make install            - Install to ~/bin"
	@echo ""
	@echo "Usage after building:"
	@echo "  ./bin/strong4vm model.uvl              # Analyze variability model"
	@echo "  ./bin/strong4vm model.uvl -t 4         # With 4 threads"
	@echo ""
	@echo "Documentation targets:"
	@echo "  make docs                - Generate complete project documentation"
	@echo "  make docs-uvl2dimacs     - Generate UVL2DIMACS documentation"
	@echo "  make docs-dimacs2graphs  - Generate Dimacs2Graphs documentation"
	@echo "  make docs-backbone       - Generate Backbone Solver documentation"
	@echo "  make docs-api            - Generate Strong4VM API documentation"
	@echo "  make clean-docs          - Clean all generated documentation"

# Documentation targets
.PHONY: docs docs-all docs-uvl2dimacs docs-dimacs2graphs docs-backbone docs-api clean-docs

docs: docs-all

docs-all:
	@echo "==========================================================================="
	@echo "Generating Complete Strong4VM Documentation"
	@echo "==========================================================================="
	@echo ""
	@command -v doxygen >/dev/null 2>&1 || { \
		echo "ERROR: doxygen not found. Install it:"; \
		echo "  Linux:  sudo apt-get install doxygen graphviz"; \
		echo "  macOS:  brew install doxygen graphviz"; \
		exit 1; \
	}
	@mkdir -p docs
	@echo "Building unified project documentation..."
	@cd docs && doxygen Doxyfile
	@echo "Removing redundant enumeration pages..."
	@rm -f docs/html/namespacemembers_enum.html
	@echo ""
	@echo "==========================================================================="
	@echo "Documentation Generation Complete"
	@echo "==========================================================================="
	@echo ""
	@echo "Main documentation: docs/html/index.html"
	@echo ""
	@echo "Component-specific documentation can be built with:"
	@echo "  make docs-uvl2dimacs    - UVL to DIMACS converter docs"
	@echo "  make docs-dimacs2graphs - Graph generator docs"
	@echo "  make docs-backbone      - Backbone solver docs"
	@echo "  make docs-api           - Strong4VM API docs"

docs-uvl2dimacs:
	@echo "Building UVL2DIMACS documentation..."
	@$(MAKE) -C $(UVL2DIMACS_DIR) docs

docs-dimacs2graphs:
	@echo "Building Dimacs2Graphs documentation..."
	@$(MAKE) -C $(DIMACS2GRAPHS_DIR) docs

docs-backbone:
	@echo "Building Backbone Solver documentation..."
	@$(MAKE) -C $(BACKBONE_SOLVER_DIR) docs

docs-api:
	@echo "Building Strong4VM API documentation..."
	@$(MAKE) -C $(API_DIR) docs

clean-docs:
	@echo "Cleaning all documentation..."
	@rm -rf docs/html
	@$(MAKE) -C $(UVL2DIMACS_DIR) clean-docs 2>/dev/null || true
	@rm -rf $(DIMACS2GRAPHS_DIR)/docs/html
	@rm -rf $(BACKBONE_SOLVER_DIR)/../docs/html
	@rm -rf $(API_DIR)/docs/html
	@echo "Documentation cleaned"

# Show build information
.PHONY: info
info:
	@echo "Build Information"
	@echo "================="
	@echo "Project Root:           $(shell pwd)"
	@echo "Strong4VM Target:       $(STRONG4VM_TARGET)"
	@echo "UVL2DIMACS Directory:   $(UVL2DIMACS_DIR)"
	@echo "Dimacs2Graphs Directory: $(DIMACS2GRAPHS_DIR)"
	@echo ""
	@echo "Build Status:"
	@echo "  Strong4VM CLI:  $$([ -f "$(STRONG4VM_TARGET)" ] && echo "Built" || echo "Not built")"
	@echo "  UVL2DIMACS:     $$([ -f "$(UVL2DIMACS_API_LIB)" ] && echo "Built" || echo "Not built")"
	@echo "  Dimacs2Graphs:  $$([ -f "$(DIMACS2GRAPHS_API)" ] && echo "Built" || echo "Not built")"
