# Strong4VM API Overview

## Purpose

The Strong4VM API provides a unified C++ interface for the complete Strong4VM pipeline, from UVL or DIMACS input to graph generation.

This is the recommended way to integrate Strong4VM into your applications.

## Design Philosophy

### Unified Interface

Single API orchestrates the complete pipeline:

```
UVL/DIMACS Input → [UVL2Dimacs] → [Dimacs2Graphs] → Graphs + Features
```

No need to manage individual components - the API handles the entire workflow.

### PIMPL Idiom

Uses Pointer-to-Implementation (PIMPL) for ABI stability:

```cpp
class Strong4VMAPI {
public:
    // Public stable interface
    APIResult analyze(const AnalysisConfig& config);

private:
    std::unique_ptr<Impl> pimpl;  // Implementation hidden
};
```

**Benefits**:
- Binary compatibility across versions
- Faster compilation
- Implementation details hidden

### Simple Error Handling

All operations return `APIResult` with:
- `success` flag
- Detailed error messages
- Result statistics

```cpp
APIResult result = api.analyze(config);
if (!result.success) {
    std::cerr << "Error: " << result.error_message << std::endl;
}
```

## Basic Usage Patterns

### Simple Analysis

Minimal configuration for quick analysis:

```cpp
#include <strong4vm/Strong4VMAPI.hh>

int main() {
    strong4vm::Strong4VMAPI api;

    // Minimal configuration
    strong4vm::AnalysisConfig config;
    config.input_file = "model.uvl";

    // Run analysis
    strong4vm::APIResult result = api.analyze(config);

    if (result.success) {
        std::cout << "Core features: " << result.num_core << std::endl;
        std::cout << "Dead features: " << result.num_dead << std::endl;
    }

    return result.success ? 0 : 1;
}
```

### Complete Configuration

Full control over analysis parameters:

```cpp
strong4vm::Strong4VMAPI api;

strong4vm::AnalysisConfig config;

// Input configuration
config.input_file = "model.uvl";
config.input_type = strong4vm::InputType::UVL;  // or InputType::DIMACS

// Output configuration
config.output_dir = "./results";
config.keep_dimacs = true;  // Keep intermediate DIMACS file

// Performance configuration
config.num_threads = 8;

// UVL-specific configuration
config.conversion_mode = strong4vm::ConversionMode::Straightforward;

// Backbone detector selection
config.detector_type = strong4vm::BackboneDetector::WithAttention;

// Run analysis
strong4vm::APIResult result = api.analyze(config);
```

## Configuration Options

### AnalysisConfig Structure

```cpp
struct AnalysisConfig {
    // Input
    std::string input_file;           // Required: path to UVL or DIMACS file
    InputType input_type;             // Auto-detected from extension if not specified

    // Output
    std::string output_dir;           // Default: same directory as input file
    bool keep_dimacs;                 // Default: false (delete intermediate file)

    // Performance
    int num_threads;                  // Default: 1

    // UVL Conversion (only for UVL input)
    ConversionMode conversion_mode;   // Straightforward (default) or Tseitin

    // Backbone Detection
    BackboneDetector detector_type;   // WithAttention (default) or WithoutAttention
};
```

### Input Types

```cpp
enum class InputType {
    UVL,      // Universal Variability Language feature model
    DIMACS,   // DIMACS CNF format
    Auto      // Auto-detect from file extension (default)
};
```

### Conversion Modes

```cpp
enum class ConversionMode {
    Straightforward,  // Default: fewer variables, potentially longer clauses
    Tseitin          // More variables, 3-CNF structure
};
```

### Backbone Detectors

```cpp
enum class BackboneDetector {
    WithAttention,        // Default: uses activity bumping (faster)
    WithoutAttention     // Baseline: no activity bumping
};
```

## Result Structure

### APIResult

```cpp
struct APIResult {
    // Success indicator
    bool success;
    std::string error_message;

    // Formula statistics
    int num_variables;
    int num_clauses;

    // Analysis results
    int num_core;              // Core (positive backbone) features
    int num_dead;              // Dead (negative backbone) features
    int num_requires_edges;    // Dependency graph edges
    int num_excludes_edges;    // Conflict graph edges

    // Timing information
    long conversion_time_ms;   // UVL to DIMACS conversion time
    long analysis_time_ms;     // Graph generation time

    // Output file paths
    std::string requires_graph_file;
    std::string excludes_graph_file;
    std::string core_features_file;
    std::string dead_features_file;
};
```

### Checking Results

```cpp
APIResult result = api.analyze(config);

if (result.success) {
    // Access statistics
    std::cout << "Formula: " << result.num_variables << " vars, "
              << result.num_clauses << " clauses" << std::endl;

    // Access results
    std::cout << "Backbone: " << result.num_core << " core, "
              << result.num_dead << " dead" << std::endl;

    // Access timing
    std::cout << "Time: " << result.analysis_time_ms << " ms" << std::endl;

    // Access output files
    std::cout << "Requires graph: " << result.requires_graph_file << std::endl;
} else {
    std::cerr << "Failed: " << result.error_message << std::endl;
}
```

## Error Handling

### Common Errors

**File Not Found**:
```cpp
if (!result.success && result.error_message.find("not found") != std::string::npos) {
    std::cerr << "Input file does not exist: " << config.input_file << std::endl;
}
```

**UNSAT Formula**:
```cpp
if (!result.success && result.error_message.find("UNSAT") != std::string::npos) {
    std::cerr << "Model has no valid configurations!" << std::endl;
}
```

**Thread Count Error**:
```cpp
if (!result.success && result.error_message.find("threads") != std::string::npos) {
    std::cerr << "Invalid thread count (max: CPU cores)" << std::endl;
}
```

### Best Practices

```cpp
APIResult result = api.analyze(config);

if (!result.success) {
    // Log full error
    std::cerr << "Analysis failed: " << result.error_message << std::endl;

    // Check specific error types
    if (result.error_message.find("UNSAT") != std::string::npos) {
        // Handle unsatisfiable formula
        return handle_unsat();
    } else if (result.error_message.find("parse") != std::string::npos) {
        // Handle parsing error
        return handle_parse_error();
    }

    return 1;
}
```

## Advanced Usage

### Auto-Detection

Input type auto-detected from extension:

```cpp
config.input_file = "model.uvl";     // Detected as UVL
config.input_type = InputType::Auto; // Auto-detect (default)

// Or

config.input_file = "formula.dimacs"; // Detected as DIMACS
```

### Batch Processing

```cpp
strong4vm::Strong4VMAPI api;  // Reuse API instance

for (const auto& file : input_files) {
    strong4vm::AnalysisConfig config;
    config.input_file = file;
    config.output_dir = create_output_dir(file);
    config.num_threads = 4;

    APIResult result = api.analyze(config);

    if (result.success) {
        std::cout << file << ": SUCCESS" << std::endl;
        log_results(result);
    } else {
        std::cerr << file << ": FAILED - " << result.error_message << std::endl;
    }
}
```

### Performance Monitoring

```cpp
#include <chrono>

auto start = std::chrono::high_resolution_clock::now();
APIResult result = api.analyze(config);
auto end = std::chrono::high_resolution_clock::now();

auto total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

std::cout << "Total time: " << total_ms << " ms" << std::endl;
std::cout << "  Conversion: " << result.conversion_time_ms << " ms" << std::endl;
std::cout << "  Analysis: " << result.analysis_time_ms << " ms" << std::endl;
std::cout << "  Overhead: " << (total_ms - result.conversion_time_ms - result.analysis_time_ms) << " ms" << std::endl;
```

## Integration

### CMake Integration

```cmake
find_library(STRONG4VM_API strong4vm_api PATHS ~/lib /usr/local/lib)
find_path(STRONG4VM_INCLUDE strong4vm/Strong4VMAPI.hh PATHS ~/include /usr/local/include)

include_directories(${STRONG4VM_INCLUDE})

add_executable(my_app main.cc)
target_link_libraries(my_app ${STRONG4VM_API})
```

### Makefile Integration

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -O3
INCLUDES = -I ~/include
LIBS = -L ~/lib -lstrong4vm_api

my_app: main.cc
\t$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(LIBS)
```

## Installation

### Building the API

```bash
# From repository root
make api              # Build API library
make api-examples     # Build API examples
make install-api      # Install to ~/lib and ~/include
```

### Installed Files

After `make install-api`:
- Library: `~/lib/libstrong4vm_api.a`
- Headers: `~/include/strong4vm/Strong4VMAPI.hh`

## Thread Safety

The Strong4VMAPI class itself is thread-safe for separate instances:

```cpp
// ✓ Safe: Each thread has its own API instance
#pragma omp parallel
{
    Strong4VMAPI api;  // Thread-local instance
    api.analyze(config);
}
```

However, **do not share a single API instance** across threads without synchronization.

## Best Practices

1. **Reuse API Instances**: For batch processing, create one API instance
2. **Check Results**: Always check `result.success` before accessing data
3. **Set Threads**: Use `config.num_threads = std::thread::hardware_concurrency() - 1`
4. **Output Directory**: Specify `output_dir` to organize results
5. **Error Handling**: Log full error messages for debugging

## Performance Tips

1. **Thread Count**: Use formula `min(cores-1, num_variables/50)`
2. **Conversion Mode**: Use `Straightforward` for simple models, `Tseitin` for complex
3. **Detector**: Stick with `WithAttention` (default) for best performance
4. **Memory**: Ensure `num_threads × 65 MB + 100 MB` RAM available

## Examples

See `examples/` directory for complete examples:
- `simple_analysis.cc` - Minimal usage
- `advanced_analysis.cc` - Full configuration
- `batch_analysis.cc` - Batch processing

Build and run:
```bash
make api-examples
./bin/simple_analysis examples/Car/model.uvl
./bin/advanced_analysis model.uvl ./output 4
```

## Related Documentation

- [Strong4VM Main Documentation](../../docs/html/index.html)
- [Getting Started](../../docs/html/getting_started.html)
- [Examples](../../docs/html/examples.html)
- API Reference: @ref strong4vm::Strong4VMAPI
