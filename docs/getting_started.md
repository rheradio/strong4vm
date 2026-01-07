# Getting Started with Strong4VM {#getting_started}

This guide will help you install, build, and run your first analysis with Strong4VM.

## Prerequisites

Before building Strong4VM, ensure you have the following installed:

- **C++17 compatible compiler**
  - g++ 7 or later
  - clang++ 5 or later
- **CMake** 3.10 or later
- **Make**
- **zlib** development libraries (required for MiniSat)

### Installing Prerequisites

**Linux (Ubuntu/Debian)**:
```bash
sudo apt-get install build-essential cmake zlib1g-dev
```

**macOS**:
```bash
brew install cmake zlib
```

## Building Strong4VM

### Quick Build

The simplest way to build Strong4VM is to run `make` from the repository root:

```bash
# Navigate to the strong4vm directory
cd strong4vm

# Build the unified CLI tool (builds all dependencies automatically)
make

# The resulting binary will be in bin/strong4vm
```

### Build Targets

Strong4VM provides several build targets:

```bash
make                # Build strong4vm CLI (default, builds all dependencies)
make install        # Install strong4vm to ~/bin
make clean          # Clean build artifacts
make clean-all      # Deep clean (including ANTLR4 and MiniSat)
make help           # Show all available targets
```

### Build Components

You can also build individual components:

```bash
# Build component APIs and CLI tools
make components      # Build UVL2Dimacs and Dimacs2Graphs components

# Build API libraries and examples
make api             # Build Strong4VM API
make api-examples    # Build API example programs
make install-api     # Install API to ~/include and ~/lib
```

### Building Documentation

Generate comprehensive Doxygen documentation:

```bash
make docs            # Generate unified project documentation
# Output: docs/html/index.html

# Or build component-specific documentation
make docs-uvl2dimacs
make docs-dimacs2graphs
make docs-backbone
make docs-api
```

**Requirements for documentation**:
- Doxygen 1.9.0+
- Graphviz (optional, for diagrams)

## Your First Analysis

### Analyzing a UVL Feature Model

The simplest way to use Strong4VM is to analyze a UVL feature model:

```bash
# Basic analysis
./bin/strong4vm model.uvl

# The tool will:
# 1. Parse the UVL file
# 2. Convert it to DIMACS CNF format
# 3. Analyze the CNF formula
# 4. Generate graphs and feature lists
```

### Analyzing a DIMACS File

You can also directly analyze DIMACS CNF formulas:

```bash
# Analyze a DIMACS file
./bin/strong4vm formula.dimacs
```

### Using Multiple Threads

For better performance on large models, use multiple threads:

```bash
# Use 8 threads for parallel graph generation
./bin/strong4vm model.uvl -t 8

# Rule of thumb for thread count:
# - Small models (<100 vars): 1-2 threads
# - Medium models (100-500 vars): 4-8 threads
# - Large models (>500 vars): up to CPU core count
```

### Customizing Output

Control where output files are generated and whether intermediate files are kept:

```bash
# Custom output directory
./bin/strong4vm model.uvl -o ./output

# Keep intermediate DIMACS file (for UVL input)
./bin/strong4vm model.uvl -k

# Combine options
./bin/strong4vm model.uvl -t 8 -o ./results -k
```

## Understanding Output Files

After running Strong4VM on `model.uvl`, you'll find these output files:

### Graph Files (Pajek Format)

- **`model__requires.net`** - Strong transitive dependency graph
  - Directed graph where an edge v → i means "if feature v is selected, then feature i must also be selected"
  - Format: Pajek .net file (can be opened with Pajek, Gephi, or NetworkX)

- **`model__excludes.net`** - Strong transitive conflict graph
  - Undirected graph where an edge v — i means "features v and i cannot both be selected"
  - Format: Pajek .net file

### Feature Lists

- **`model__core.txt`** - Core features
  - Lists features that are enabled in all valid configurations
  - These features form the positive backbone

- **`model__dead.txt`** - Dead features
  - Lists features that are disabled in all valid configurations
  - These features form the negative backbone

### Intermediate Files (if -k flag used)

- **`model.dimacs`** - DIMACS CNF representation (UVL input only)
  - Standard SAT solver format
  - Contains comments mapping variables to feature names

## Command-Line Options

```bash
strong4vm <input_file> [options]
```

**Options**:
- `-t, --threads N` - Number of threads for graph generation (default: 1)
- `-o, --output DIR` - Output directory (default: same as input file)
- `-k, --keep-dimacs` - Keep intermediate DIMACS file (UVL input only)
- `-h, --help` - Display help message and exit

## Example Workflow

Here's a complete example workflow:

```bash
# 1. Navigate to the repository
cd strong4vm

# 2. Build the tool
make

# 3. Try an example model (45+ examples included)
./bin/strong4vm examples/Car/model.uvl -t 4 -o ./output

# 4. View the results
ls -lh output/
# You'll see:
#   model__requires.net
#   model__excludes.net
#   model__core.txt
#   model__dead.txt

# 5. Check core features
cat output/model__core.txt

# 6. Check dead features
cat output/model__dead.txt
```

## Verifying Installation

To verify your installation is working correctly:

```bash
# Show help message
./bin/strong4vm --help

# Try a small example
./bin/strong4vm examples/HelloWorld/model.uvl

# Check that output files were created
ls examples/HelloWorld/model__*
```

## Troubleshooting

### Build Issues

**Problem**: CMake or build errors

**Solution**:
```bash
make clean-all  # Deep clean
make            # Rebuild from scratch
```

**Problem**: Missing dependencies

**Solution**: Install required packages (see Prerequisites section above)

### Runtime Issues

**Problem**: "Formula is UNSAT" error

**Solution**: This means your feature model has contradictory constraints and no valid configuration exists. Review your model's constraints.

**Problem**: Out of memory errors

**Solution**: Reduce the number of threads with the `-t` option, or analyze on a machine with more RAM (each thread requires ~60-70 MB).

## Next Steps

- See @ref examples for more usage patterns
- Read @ref architecture to understand how Strong4VM works internally
- Review @ref limitations for known limitations
- Explore the API documentation for programmatic usage: @ref strong4vm::Strong4VMAPI

## Getting Help

- Check the [README](../README.md) for additional information
- Visit the [GitHub repository](https://github.com/rheradio/Strong4VM) to report issues
- Review @ref limitations for common issues and workarounds
