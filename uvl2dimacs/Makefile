# Makefile for UVL to DIMACS Translator
# Builds ANTLR4 C++ runtime, UVL parser, and uvl2dimacs

# Directories
PROJECT_ROOT := $(shell pwd)
THIRD_PARTY := $(PROJECT_ROOT)/third_party
ANTLR4_DIR := $(THIRD_PARTY)
ANTLR4_BUILD := $(ANTLR4_DIR)/build
ANTLR4_LIB := $(ANTLR4_BUILD)/runtime/libantlr4-runtime.a
UVL_PARSER_DIR := $(PROJECT_ROOT)/parser
UVL_CPP_DIR := $(UVL_PARSER_DIR)
BUILD_DIR := $(PROJECT_ROOT)/build

# Build flags - optimized for maximum performance
# -O3: Maximum optimization level
# -march=native: Optimize for the current CPU architecture
# -mtune=native: Tune code for the current CPU
# -DNDEBUG: Disable assertions
# -flto: Link-time optimization for cross-module optimization
# CMAKE_INTERPROCEDURAL_OPTIMIZATION: Enable LTO at CMake level
# Note: LTO disabled on macOS to avoid static library linking issues

# Detect OS
UNAME_S := $(shell uname -s)

# Set flags based on OS
ifeq ($(UNAME_S),Darwin)
    # macOS: Disable LTO to prevent archive linking issues
    CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Release \
                   -DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native -mtune=native -DNDEBUG" \
                   -DCMAKE_C_FLAGS_RELEASE="-O3 -march=native -mtune=native -DNDEBUG"

    ANTLR4_CMAKE_FLAGS := -DANTLR_BUILD_CPP_TESTS=OFF \
                          -DANTLR_BUILD_SHARED=OFF \
                          -DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native -mtune=native -DNDEBUG" \
                          -DCMAKE_C_FLAGS_RELEASE="-O3 -march=native -mtune=native -DNDEBUG"
else
    # Linux/other: Enable LTO for maximum performance
    CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Release \
                   -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON \
                   -DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native -mtune=native -DNDEBUG -flto" \
                   -DCMAKE_C_FLAGS_RELEASE="-O3 -march=native -mtune=native -DNDEBUG -flto"

    ANTLR4_CMAKE_FLAGS := -DANTLR_BUILD_CPP_TESTS=OFF \
                          -DANTLR_BUILD_SHARED=OFF \
                          -DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native -mtune=native -DNDEBUG -flto" \
                          -DCMAKE_C_FLAGS_RELEASE="-O3 -march=native -mtune=native -DNDEBUG -flto"
endif

# Output directories
DOCS_DIR := $(PROJECT_ROOT)/docs

# Default target
.PHONY: all
all: antlr4-runtime uvl2dimacs api examples

# Help target
.PHONY: help
help:
	@echo "UVL to DIMACS Translator - Build System"
	@echo "========================================"
	@echo ""
	@echo "Targets:"
	@echo "  all              - Build everything (default)"
	@echo "  antlr4-runtime   - Build ANTLR4 C++ runtime"
	@echo "  uvl2dimacs       - Build uvl2dimacs translator"
	@echo "  api              - Build API library"
	@echo "  examples         - Build API examples (simple_convert, batch_convert)"
	@echo "  test             - Build and run tests"
	@echo "  clean            - Clean build artifacts"
	@echo "  clean-all        - Clean everything including ANTLR4"
	@echo "  clean-docs       - Clean documentation"
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
examples: $(BUILD_DIR)/simple_convert $(BUILD_DIR)/batch_convert

$(BUILD_DIR)/simple_convert: api
	@echo "========================================="
	@echo "Building API Examples..."
	@echo "========================================="
	@# Examples are built as part of the main build
	@if [ -f "$(BUILD_DIR)/simple_convert" ] && [ -f "$(BUILD_DIR)/batch_convert" ]; then \
		echo "API examples built successfully"; \
		echo "  - $(BUILD_DIR)/simple_convert"; \
		echo "  - $(BUILD_DIR)/batch_convert"; \
	else \
		echo "Warning: Examples not found, they should be built with uvl2dimacs"; \
	fi

$(BUILD_DIR)/batch_convert: api
	@# This is a placeholder - batch_convert is built together with simple_convert

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

# Clean everything including ANTLR4
.PHONY: clean-all
clean-all: clean clean-docs
	@echo "Cleaning ANTLR4 runtime..."
	@rm -rf $(ANTLR4_BUILD)
	@echo "Clean all complete"

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
	@command -v cmake >/dev/null 2>&1 || { echo "ERROR: cmake not found"; exit 1; }
	@command -v g++ >/dev/null 2>&1 || { echo "ERROR: g++ not found"; exit 1; }
	@command -v make >/dev/null 2>&1 || { echo "ERROR: make not found"; exit 1; }
	@echo "All required dependencies found"
	@echo ""
	@echo "Optional dependencies:"
	@command -v doxygen >/dev/null 2>&1 && echo "  doxygen: Found" || echo "  doxygen: Not found (needed for 'make docs')"

# Show build information
.PHONY: info
info:
	@echo "Build Information"
	@echo "================="
	@echo "Project Root:    $(PROJECT_ROOT)"
	@echo "Build Directory: $(BUILD_DIR)"
	@echo "ANTLR4 Runtime:  $(ANTLR4_DIR)"
	@echo "ANTLR4 Library:  $(ANTLR4_LIB)"
	@echo "UVL Parser:      $(UVL_PARSER_DIR)"
	@echo ""
	@echo "Build Status:"
	@echo "  ANTLR4 Runtime: $$([ -f "$(ANTLR4_LIB)" ] && echo "Built" || echo "Not built")"
	@echo "  UVL Parser:     $$([ -d "$(UVL_CPP_DIR)/include" ] && echo "Generated" || echo "Not generated")"
	@echo "  uvl2dimacs:     $$([ -f "$(BUILD_DIR)/uvl2dimacs" ] && echo "Built" || echo "Not built")"
	@echo "  Documentation:  $$([ -d "$(DOCS_DIR)/html" ] && echo "Generated" || echo "Not generated")"
