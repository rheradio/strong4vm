
PROGRAM_NAME := backbone_solver

#Source directories
CLI_DIR := cli
API_DIR := api
DETECTORS_DIR := detectors
CORE_DIR := data_structures
IO_DIR := io
MINISAT_INTERFACE_DIR := minisat_interface

#Source lists with proper paths
MAIN_SRCS := $(CLI_DIR)/main.cc $(API_DIR)/BoneDiggerAPI.cc $(IO_DIR)/DIMACSReader.cc \
             $(DETECTORS_DIR)/CheckCandidatesOneByOne.cc \
             $(DETECTORS_DIR)/FastOnCliffsSlowOnPlains.cc $(DETECTORS_DIR)/RushAndPray.cc \
             $(CORE_DIR)/LiteralSet.cc $(MINISAT_INTERFACE_DIR)/minisat_aux.cc

API_EXAMPLE_SRCS := $(API_DIR)/api_example.cc $(API_DIR)/BoneDiggerAPI.cc $(IO_DIR)/DIMACSReader.cc \
                    $(DETECTORS_DIR)/CheckCandidatesOneByOne.cc \
                    $(DETECTORS_DIR)/FastOnCliffsSlowOnPlains.cc $(DETECTORS_DIR)/RushAndPray.cc \
                    $(CORE_DIR)/LiteralSet.cc $(MINISAT_INTERFACE_DIR)/minisat_aux.cc

MAIN_OBJS := $(MAIN_SRCS:.cc=.o)
API_EXAMPLE_OBJS := $(API_EXAMPLE_SRCS:.cc=.o)
DEPENDS  := $(wildcard */*.d)

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
# Compiler Detection and Selection
# ============================================================

# Compiler - select based on OS if not explicitly set
ifeq ($(OS),macos)
    CXX ?= clang++
else
    CXX ?= g++
endif

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

# ============================================================
# Architecture-specific optimization flags
# ============================================================

ifeq ($(ARCH),x86_64)
    ifeq ($(CPU_TYPE),xeon)
        # Xeon with AVX-512 support - use native for best performance
        ARCH_FLAGS := -march=native -mtune=native
    else
        # Generic x86_64 - use x86-64-v2 if the compiler supports it (GCC 11+),
        # otherwise fall back to plain x86-64 for older toolchains
        _MARCH_TEST := $(shell echo "" | $(CXX) -march=x86-64-v2 -x c++ - -fsyntax-only 2>&1)
        ifeq ($(findstring bad value,$(_MARCH_TEST)),)
            ARCH_FLAGS := -march=x86-64-v2 -mtune=generic
        else
            ARCH_FLAGS := -march=x86-64 -mtune=generic
        endif
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

# Base flags (common to both compilers)
CCFLAGS := -O3 $(ARCH_FLAGS) -ffast-math -DNDEBUG
CCFLAGS += -funroll-loops -finline-functions -fomit-frame-pointer -fmerge-all-constants
CCFLAGS += -fno-signed-zeros -fno-trapping-math -fno-math-errno -ffinite-math-only
CCFLAGS += -fno-stack-protector -falign-functions=32
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
    CCFLAGS += -falign-loops=32

    # GCC-specific linker flags
    ifeq ($(shell uname),Darwin)
        LNFLAGS := -flto=auto -fuse-linker-plugin -Wl,-dead_strip
    else
        LNFLAGS := -flto=auto -fuse-linker-plugin -Wl,-O3
        LNFLAGS += -Wl,--as-needed -Wl,--sort-common -Wl,--gc-sections
        LNFLAGS += -Wl,--hash-style=gnu -Wl,--build-id=none
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

.PHONY: all clean api api_example distclean docs show-compiler
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

# Pattern rule for compiling .cc -> .o
%.o: %.cc
	@echo Compiling $@
	$(CXX) $(CCFLAGS) -c -o $@ $<

clean:
	@echo Cleaning build artifacts...
	@rm -f $(CLI_DIR)/*.o $(API_DIR)/*.o $(DETECTORS_DIR)/*.o $(CORE_DIR)/*.o $(IO_DIR)/*.o $(MINISAT_INTERFACE_DIR)/*.o
	@rm -f $(CLI_DIR)/*.d $(API_DIR)/*.d $(DETECTORS_DIR)/*.d $(CORE_DIR)/*.d $(IO_DIR)/*.d $(MINISAT_INTERFACE_DIR)/*.d
	@rm -f $(BIN_DIR)/$(PROGRAM_NAME) $(BIN_DIR)/api_example
	@rm -rf $(BIN_DIR)
	@echo Cleaning MiniSat build artifacts...
	@cd minisat && $(MAKE) clean || true

distclean: clean

docs:
	@echo "Generating documentation with Doxygen..."
	@cd .. && doxygen docs/Doxyfile
	@echo "Documentation generated in docs/ directory"
	@echo "Open docs/html/index.html in a browser to view"

-include $(DEPENDS)