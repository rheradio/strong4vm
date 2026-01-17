# Makefile for UVL to DIMACS Translator
# Builds ANTLR4 C++ runtime, UVL parser, and uvl2dimacs
# Supports macOS and Linux on x86_64, ARM64, and Xeon processors

# Directories
PROJECT_ROOT := $(shell pwd)
THIRD_PARTY := $(PROJECT_ROOT)/third_party
ANTLR4_DIR := $(THIRD_PARTY)
ANTLR4_BUILD := $(ANTLR4_DIR)/build
ANTLR4_LIB := $(ANTLR4_BUILD)/runtime/libantlr4-runtime.a
UVL_PARSER_DIR := $(PROJECT_ROOT)/parser
UVL_CPP_DIR := $(UVL_PARSER_DIR)
BUILD_DIR := $(PROJECT_ROOT)/build

# ============================================================
# OS and Architecture Detection
# ============================================================

# Detect operating system
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    OS := macos
else ifeq ($(UNAME_S),Linux)
    OS := linux
else
    OS := unknown
endif

# Detect processor architecture
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),x86_64)
    ARCH := x86_64
else ifeq ($(UNAME_M),amd64)
    ARCH := x86_64
else ifeq ($(UNAME_M),arm64)
    ARCH := arm64
else ifeq ($(UNAME_M),aarch64)
    ARCH := arm64
else ifeq ($(UNAME_M),i386)
    ARCH := x86
else ifeq ($(UNAME_M),i686)
    ARCH := x86
else
    ARCH := unknown
endif

# Detect if running on Xeon (Intel server CPU with AVX-512 support)
# Check for AVX-512 in /proc/cpuinfo (Linux) or sysctl (macOS)
ifeq ($(ARCH),x86_64)
    ifeq ($(OS),linux)
        HAS_AVX512 := $(shell grep -q "avx512" /proc/cpuinfo 2>/dev/null && echo yes || echo no)
    else ifeq ($(OS),macos)
        HAS_AVX512 := $(shell sysctl -a 2>/dev/null | grep -q "hw.optional.avx512" && echo yes || echo no)
    endif
    ifeq ($(HAS_AVX512),yes)
        CPU_TYPE := xeon
    else
        CPU_TYPE := generic_x86_64
    endif
else ifeq ($(ARCH),arm64)
    CPU_TYPE := arm64
else
    CPU_TYPE := generic
endif

# ============================================================
# Architecture-specific compiler flags
# ============================================================

# Base optimization flags (common to all architectures)
BASE_OPT_FLAGS := -O3 -DNDEBUG

# Architecture-specific optimization flags
ifeq ($(ARCH),x86_64)
    ifeq ($(CPU_TYPE),xeon)
        # Xeon with AVX-512 support
        ARCH_FLAGS := -march=native -mtune=native
    else
        # Generic x86_64 - use baseline that works everywhere
        # Use -march=x86-64-v2 for better compatibility (SSE4.2, POPCNT)
        ARCH_FLAGS := -march=x86-64-v2 -mtune=generic
    endif
else ifeq ($(ARCH),arm64)
    ifeq ($(OS),macos)
        # Apple Silicon (M1/M2/M3)
        ARCH_FLAGS := -mcpu=apple-m1
    else
        # Generic ARM64 (Linux)
        ARCH_FLAGS := -march=armv8-a -mtune=generic
    endif
else ifeq ($(ARCH),x86)
    # 32-bit x86
    ARCH_FLAGS := -march=i686 -mtune=generic
else
    # Fallback - no specific architecture optimization
    ARCH_FLAGS :=
endif

# LTO flags - different between compilers
ifeq ($(OS),macos)
    # macOS uses Apple Clang which supports thin LTO
    LTO_FLAGS := -flto=thin
else
    # Linux - check for GCC vs Clang
    COMPILER_VERSION := $(shell $(CXX) --version 2>/dev/null)
    ifneq (,$(findstring clang,$(COMPILER_VERSION)))
        LTO_FLAGS := -flto=thin
    else
        # GCC uses regular LTO (thin LTO not supported)
        LTO_FLAGS := -flto
    endif
endif

# Combined optimization flags
OPT_FLAGS := $(BASE_OPT_FLAGS) $(ARCH_FLAGS) $(LTO_FLAGS)

# Build flags - optimized for maximum performance
# -O3: Maximum optimization level
# Architecture-specific flags based on detected CPU
# -DNDEBUG: Disable assertions
# -flto: Link-time optimization for cross-module optimization
# CMAKE_INTERPROCEDURAL_OPTIMIZATION: Enable LTO at CMake level
CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Release \
               -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON \
               -DCMAKE_CXX_FLAGS_RELEASE="$(OPT_FLAGS)" \
               -DCMAKE_C_FLAGS_RELEASE="$(OPT_FLAGS)"

ANTLR4_CMAKE_FLAGS := -DANTLR_BUILD_CPP_TESTS=OFF \
                      -DANTLR_BUILD_SHARED=OFF \
                      -DCMAKE_CXX_FLAGS_RELEASE="$(OPT_FLAGS)" \
                      -DCMAKE_C_FLAGS_RELEASE="$(OPT_FLAGS)"

# Output directories
DOCS_DIR := $(PROJECT_ROOT)/docs

# Default target
.PHONY: all
all: antlr4-runtime uvl2dimacs api

# Help target
.PHONY: help
help:
	@echo "UVL to DIMACS Translator - Build System"
	@echo "========================================"
	@echo "Supports: macOS/Linux on x86_64, ARM64, Xeon"
	@echo ""
	@echo "Build Targets:"
	@echo "  all              - Build everything (default)"
	@echo "  antlr4-runtime   - Build ANTLR4 C++ runtime"
	@echo "  uvl2dimacs       - Build uvl2dimacs translator"
	@echo "  api              - Build API library"
	@echo "  examples         - Build API examples (simple_convert, tseitin_convert, batch_convert)"
	@echo "  backbone_solver  - Build backbone solver for tests"
	@echo ""
	@echo "Test Targets:"
	@echo "  test             - Build and run tests"
	@echo ""
	@echo "Clean Targets:"
	@echo "  clean            - Clean build artifacts"
	@echo "  clean-all        - Clean everything including all dependencies"
	@echo "  clean-backbone   - Clean backbone solver (including MiniSat)"
	@echo "  clean-docs       - Clean documentation"
	@echo ""
	@echo "Info Targets:"
	@echo "  info             - Show detected OS, architecture, and build status"
	@echo "  check-deps       - Check for required dependencies"
	@echo "  install          - Install uvl2dimacs binary and examples"
	@echo ""
	@echo "Example Usage:"
	@echo "  make              # Build everything"
	@echo "  make examples     # Build API examples"
	@echo "  make test         # Build and test"
	@echo "  make clean all    # Rebuild from scratch"

# Build ANTLR4 C++ runtime
.PHONY: antlr4-runtime
antlr4-runtime: $(ANTLR4_LIB)

$(ANTLR4_LIB):
	@echo "========================================="
	@echo "Building ANTLR4 C++ Runtime..."
	@echo "========================================="
	@if [ ! -f "$(ANTLR4_DIR)/CMakeLists.txt" ]; then \
		echo "Error: ANTLR4 source not found in $(ANTLR4_DIR)"; \
		echo "Please ensure antlr4-cpp-runtime is extracted to third_party/"; \
		exit 1; \
	fi
	@mkdir -p $(ANTLR4_BUILD)
	@cd $(ANTLR4_BUILD) && \
		cmake $(ANTLR4_CMAKE_FLAGS) $(CMAKE_FLAGS) .. && \
		$(MAKE) -j$(shell sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)
	@echo "ANTLR4 runtime built successfully: $(ANTLR4_LIB)"

# Build uvl2dimacs translator (parser is built inline now)
.PHONY: uvl2dimacs
uvl2dimacs: $(BUILD_DIR)/uvl2dimacs

$(BUILD_DIR)/uvl2dimacs: antlr4-runtime
	@echo "========================================="
	@echo "Building UVL to DIMACS Translator..."
	@echo "========================================="
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
		cmake -DANTLR4_INCLUDE_DIR=$(ANTLR4_DIR)/runtime/src \
		      -DANTLR4_LIBRARY=$(ANTLR4_LIB) \
		      $(CMAKE_FLAGS) .. && \
		$(MAKE) -j$(shell sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)
	@echo "========================================="
	@echo "Build Complete!"
	@echo "========================================="
	@echo "Executable: $(BUILD_DIR)/uvl2dimacs"
	@echo ""
	@echo "Usage: $(BUILD_DIR)/uvl2dimacs <input.uvl> <output.dimacs>"

# Build API library
.PHONY: api
api: $(BUILD_DIR)/libuvl2dimacs_api.a

$(BUILD_DIR)/libuvl2dimacs_api.a: antlr4-runtime uvl2dimacs
	@echo "========================================="
	@echo "Building API Library..."
	@echo "========================================="
	@# API is built as part of the main build
	@if [ -f "$(BUILD_DIR)/libuvl2dimacs_api.a" ]; then \
		echo "API library built successfully"; \
	else \
		echo "Warning: API library not found, it should be built with uvl2dimacs"; \
	fi

# Build API examples
.PHONY: examples
examples: $(BUILD_DIR)/simple_convert $(BUILD_DIR)/tseitin_convert $(BUILD_DIR)/batch_convert

$(BUILD_DIR)/simple_convert: api
	@echo "========================================="
	@echo "Building API Examples..."
	@echo "========================================="
	@# Examples are built as part of the main build
	@if [ -f "$(BUILD_DIR)/simple_convert" ] && [ -f "$(BUILD_DIR)/tseitin_convert" ] && [ -f "$(BUILD_DIR)/batch_convert" ]; then \
		echo "API examples built successfully"; \
		echo "  - $(BUILD_DIR)/simple_convert"; \
		echo "  - $(BUILD_DIR)/tseitin_convert"; \
		echo "  - $(BUILD_DIR)/batch_convert"; \
	else \
		echo "Warning: Examples not found, they should be built with uvl2dimacs"; \
	fi

$(BUILD_DIR)/tseitin_convert: api
	@# This is a placeholder - tseitin_convert is built together with simple_convert

$(BUILD_DIR)/batch_convert: api
	@# This is a placeholder - batch_convert is built together with simple_convert

# Build backbone solver for tests
BACKBONE_DIR := $(PROJECT_ROOT)/backbone_solver
BACKBONE_SRC := $(BACKBONE_DIR)/src
BACKBONE_BIN := $(BACKBONE_DIR)/bin/backbone_solver

.PHONY: backbone_solver
backbone_solver: $(BACKBONE_BIN)

$(BACKBONE_BIN):
	@echo "========================================="
	@echo "Building Backbone Solver..."
	@echo "========================================="
	@if [ ! -f "$(BACKBONE_SRC)/Makefile" ]; then \
		echo "Error: Backbone solver source not found in $(BACKBONE_SRC)"; \
		exit 1; \
	fi
	@# Detect compiler based on OS
	@if [ "$$(uname)" = "Darwin" ]; then \
		CXX_COMPILER="clang++"; \
	elif command -v g++ >/dev/null 2>&1; then \
		CXX_COMPILER="g++"; \
	elif command -v clang++ >/dev/null 2>&1; then \
		CXX_COMPILER="clang++"; \
	else \
		echo "Error: No C++ compiler found (tried g++ and clang++)"; \
		exit 1; \
	fi && \
	cd $(BACKBONE_SRC) && $(MAKE) CXX="$$CXX_COMPILER"
	@echo "Backbone solver built successfully: $(BACKBONE_BIN)"
	@echo ""

# Build and run tests
.PHONY: test
test: uvl2dimacs
	@echo "========================================="
	@echo "Running Tests..."
	@echo "========================================="
	@cd $(PROJECT_ROOT)/test && ./run_tests.sh
	@echo ""
	@echo "Tests completed successfully!"

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
	@cd docs && doxygen Doxyfile
	@echo ""
	@echo "Documentation generated in docs/html/"
	@echo "Open docs/html/index.html in your browser"

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
	@rm -rf $(PROJECT_ROOT)/CMakeFiles
	@rm -f $(PROJECT_ROOT)/CMakeCache.txt
	@rm -f $(PROJECT_ROOT)/cmake_install.cmake
	@rm -f $(PROJECT_ROOT)/*.cmake
	@rm -f test_output.dimacs
	@echo "Clean complete"

# Clean everything including ANTLR4 and all dependencies
.PHONY: clean-all
clean-all: clean clean-docs clean-backbone
	@echo "Cleaning ANTLR4 runtime..."
	@rm -rf $(ANTLR4_BUILD)
	@echo "Clean all complete"

# Clean only backbone solver (including MiniSat)
.PHONY: clean-backbone
clean-backbone:
	@echo "Cleaning Backbone Solver (including MiniSat)..."
	@if [ -f "$(BACKBONE_SRC)/Makefile" ]; then \
		cd $(BACKBONE_SRC) && $(MAKE) distclean 2>/dev/null || true; \
	fi

# Clean documentation
.PHONY: clean-docs
clean-docs:
	@echo "Cleaning documentation..."
	@rm -rf $(DOCS_DIR)/html
	@echo "Documentation cleaned"

# Install uvl2dimacs and examples
.PHONY: install
install: uvl2dimacs examples
	@echo "Installing uvl2dimacs and examples..."
	@mkdir -p $(HOME)/bin
	@cp $(BUILD_DIR)/uvl2dimacs $(HOME)/bin/
	@if [ -f "$(BUILD_DIR)/simple_convert" ]; then \
		cp $(BUILD_DIR)/simple_convert $(HOME)/bin/; \
		echo "Installed simple_convert to $(HOME)/bin/"; \
	fi
	@if [ -f "$(BUILD_DIR)/tseitin_convert" ]; then \
		cp $(BUILD_DIR)/tseitin_convert $(HOME)/bin/; \
		echo "Installed tseitin_convert to $(HOME)/bin/"; \
	fi
	@if [ -f "$(BUILD_DIR)/batch_convert" ]; then \
		cp $(BUILD_DIR)/batch_convert $(HOME)/bin/; \
		echo "Installed batch_convert to $(HOME)/bin/"; \
	fi
	@echo "Installed to $(HOME)/bin/uvl2dimacs"
	@echo "Make sure $(HOME)/bin is in your PATH"

# Check dependencies
.PHONY: check-deps
check-deps:
	@echo "Checking dependencies..."
	@echo ""
	@echo "System: $(OS) $(ARCH) ($(CPU_TYPE))"
	@echo ""
	@echo "Required build tools:"
	@command -v cmake >/dev/null 2>&1 && echo "  cmake: Found" || { echo "  cmake: NOT FOUND - please install cmake"; exit 1; }
	@command -v make >/dev/null 2>&1 && echo "  make: Found" || { echo "  make: NOT FOUND - please install make"; exit 1; }
	@(command -v g++ >/dev/null 2>&1 && echo "  g++: Found") || \
		(command -v clang++ >/dev/null 2>&1 && echo "  clang++: Found") || \
		{ echo "  C++ compiler: NOT FOUND - please install g++ or clang++"; exit 1; }
	@echo ""
	@echo "Optional dependencies:"
	@command -v doxygen >/dev/null 2>&1 && echo "  doxygen: Found" || echo "  doxygen: Not found (needed for 'make docs')"

# Show build information
.PHONY: info
info:
	@echo "Build Information"
	@echo "================="
	@echo ""
	@echo "System:"
	@echo "  Operating System:  $(OS) ($(UNAME_S))"
	@echo "  Architecture:      $(ARCH) ($(UNAME_M))"
	@echo "  CPU Type:          $(CPU_TYPE)"
	@echo "  Compiler Flags:    $(ARCH_FLAGS)"
	@echo "  LTO Flags:         $(LTO_FLAGS)"
	@echo ""
	@echo "Directories:"
	@echo "  Project Root:      $(PROJECT_ROOT)"
	@echo "  Build Directory:   $(BUILD_DIR)"
	@echo "  ANTLR4 Runtime:    $(ANTLR4_DIR)"
	@echo "  ANTLR4 Library:    $(ANTLR4_LIB)"
	@echo "  UVL Parser:        $(UVL_PARSER_DIR)"
	@echo ""
	@echo "Build Status:"
	@echo "  ANTLR4 Runtime:    $$([ -f "$(ANTLR4_LIB)" ] && echo "Built" || echo "Not built")"
	@echo "  UVL Parser:        $$([ -d "$(UVL_CPP_DIR)/include" ] && echo "Generated" || echo "Not generated")"
	@echo "  uvl2dimacs:        $$([ -f "$(BUILD_DIR)/uvl2dimacs" ] && echo "Built" || echo "Not built")"
	@echo "  Backbone Solver:   $$([ -f "$(BACKBONE_BIN)" ] && echo "Built" || echo "Not built")"
	@echo "  Documentation:     $$([ -d "$(DOCS_DIR)/html" ] && echo "Generated" || echo "Not generated")"
