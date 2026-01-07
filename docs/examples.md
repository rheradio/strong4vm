# Strong4VM Usage Examples {#examples}

This page provides comprehensive examples of using Strong4VM via both the CLI and the C++ API.

## CLI Examples

### Basic Usage

**Simple analysis of a UVL model**:
```bash
./bin/strong4vm examples/Car/model.uvl
```

This will generate:
- `examples/Car/model__requires.net` - Dependency graph
- `examples/Car/model__excludes.net` - Conflict graph
- `examples/Car/model__core.txt` - Core features
- `examples/Car/model__dead.txt` - Dead features

**Analyze a DIMACS file**:
```bash
./bin/strong4vm formula.dimacs
```

### Advanced CLI Usage

**Multi-threaded analysis**:
```bash
# Use 8 threads for faster processing
./bin/strong4vm model.uvl -t 8
```

**Custom output directory**:
```bash
# Place all output in ./results/
./bin/strong4vm model.uvl -o ./results
```

**Keep intermediate DIMACS file**:
```bash
# Keep the .dimacs file for inspection
./bin/strong4vm model.uvl -k
```

**Combined options**:
```bash
# Full-featured analysis
./bin/strong4vm examples/Car/model.uvl -t 4 -o ./output -k
```

### Batch Processing

**Process multiple models**:
```bash
# Process all UVL models in a directory
for file in examples/**/model.uvl; do
    echo "Processing $file..."
    ./bin/strong4vm "$file" -t 4
done
```

## API Examples

### Simple Analysis (Complete Pipeline)

The unified API provides the easiest way to run the complete pipeline:

```cpp
#include <iostream>
#include <strong4vm/Strong4VMAPI.hh>

int main() {
    strong4vm::Strong4VMAPI api;

    // Configure analysis
    strong4vm::AnalysisConfig config;
    config.input_file = "model.uvl";
    config.output_dir = "./output";
    config.num_threads = 4;
    config.keep_dimacs = false;

    // Run analysis
    strong4vm::APIResult result = api.analyze(config);

    // Check results
    if (result.success) {
        std::cout << "Analysis completed successfully!" << std::endl;
        std::cout << "Total variables: " << result.num_variables << std::endl;
        std::cout << "Total clauses: " << result.num_clauses << std::endl;
        std::cout << "Core features: " << result.num_core << std::endl;
        std::cout << "Dead features: " << result.num_dead << std::endl;
        std::cout << "Requires edges: " << result.num_requires_edges << std::endl;
        std::cout << "Excludes edges: " << result.num_excludes_edges << std::endl;
    } else {
        std::cerr << "Analysis failed: " << result.error_message << std::endl;
        return 1;
    }

    return 0;
}
```

### Advanced Analysis with Custom Configuration

```cpp
#include <strong4vm/Strong4VMAPI.hh>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file> [output_dir] [num_threads]" << std::endl;
        return 1;
    }

    // Parse arguments
    std::string input_file = argv[1];
    std::string output_dir = argc > 2 ? argv[2] : fs::path(input_file).parent_path();
    int num_threads = argc > 3 ? std::stoi(argv[3]) : 1;

    // Create API instance
    strong4vm::Strong4VMAPI api;

    // Configure analysis
    strong4vm::AnalysisConfig config;
    config.input_file = input_file;
    config.output_dir = output_dir;
    config.num_threads = num_threads;
    config.keep_dimacs = true;  // Keep intermediate file

    // Auto-detect input type
    if (input_file.ends_with(".uvl")) {
        config.input_type = strong4vm::InputType::UVL;
        config.conversion_mode = strong4vm::ConversionMode::Straightforward;
    } else if (input_file.ends_with(".dimacs") || input_file.ends_with(".cnf")) {
        config.input_type = strong4vm::InputType::DIMACS;
    } else {
        std::cerr << "Unknown file type. Supported: .uvl, .dimacs, .cnf" << std::endl;
        return 1;
    }

    // Run analysis
    std::cout << "Analyzing " << input_file << "..." << std::endl;
    std::cout << "Using " << num_threads << " thread(s)" << std::endl;

    strong4vm::APIResult result = api.analyze(config);

    // Display detailed results
    if (result.success) {
        std::cout << "\n=== Analysis Results ===" << std::endl;
        std::cout << "Input file: " << input_file << std::endl;
        std::cout << "Output directory: " << output_dir << std::endl;
        std::cout << "\nFormula Statistics:" << std::endl;
        std::cout << "  Variables: " << result.num_variables << std::endl;
        std::cout << "  Clauses: " << result.num_clauses << std::endl;
        std::cout << "\nBackbone Analysis:" << std::endl;
        std::cout << "  Core features: " << result.num_core << std::endl;
        std::cout << "  Dead features: " << result.num_dead << std::endl;
        std::cout << "\nGraph Generation:" << std::endl;
        std::cout << "  Requires edges: " << result.num_requires_edges << std::endl;
        std::cout << "  Excludes edges: " << result.num_excludes_edges << std::endl;
        std::cout << "\nExecution Time:" << std::endl;
        std::cout << "  Conversion: " << result.conversion_time_ms << " ms" << std::endl;
        std::cout << "  Analysis: " << result.analysis_time_ms << " ms" << std::endl;
        std::cout << "  Total: " << (result.conversion_time_ms + result.analysis_time_ms) << " ms" << std::endl;
    } else {
        std::cerr << "\nAnalysis failed!" << std::endl;
        std::cerr << "Error: " << result.error_message << std::endl;
        return 1;
    }

    return 0;
}
```

### Batch Processing with API

```cpp
#include <strong4vm/Strong4VMAPI.hh>
#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_dir> <output_dir> [num_threads]" << std::endl;
        return 1;
    }

    std::string input_dir = argv[1];
    std::string output_dir = argv[2];
    int num_threads = argc > 3 ? std::stoi(argv[3]) : 1;

    // Find all UVL files
    std::vector<fs::path> uvl_files;
    for (const auto& entry : fs::recursive_directory_iterator(input_dir)) {
        if (entry.path().extension() == ".uvl") {
            uvl_files.push_back(entry.path());
        }
    }

    std::cout << "Found " << uvl_files.size() << " UVL files" << std::endl;

    // Process each file
    strong4vm::Strong4VMAPI api;
    int success_count = 0;
    int failure_count = 0;

    for (const auto& file : uvl_files) {
        std::cout << "\nProcessing: " << file.filename() << "..." << std::endl;

        // Create subdirectory for this model
        fs::path model_output = fs::path(output_dir) / file.stem();
        fs::create_directories(model_output);

        // Configure analysis
        strong4vm::AnalysisConfig config;
        config.input_file = file.string();
        config.output_dir = model_output.string();
        config.num_threads = num_threads;

        // Run analysis
        strong4vm::APIResult result = api.analyze(config);

        if (result.success) {
            std::cout << "  ✓ Success - Core: " << result.num_core
                      << ", Dead: " << result.num_dead << std::endl;
            success_count++;
        } else {
            std::cout << "  ✗ Failed: " << result.error_message << std::endl;
            failure_count++;
        }
    }

    // Summary
    std::cout << "\n=== Batch Processing Complete ===" << std::endl;
    std::cout << "Total files: " << uvl_files.size() << std::endl;
    std::cout << "Successful: " << success_count << std::endl;
    std::cout << "Failed: " << failure_count << std::endl;

    return (failure_count > 0) ? 1 : 0;
}
```

## Component-Specific Examples

### UVL to DIMACS Conversion Only

```cpp
#include <uvl2dimacs/UVL2Dimacs.hh>
#include <iostream>

int main() {
    // Create converter instance
    uvl2dimacs::UVL2DimacsAPI converter;

    // Simple conversion
    bool success = converter.convertFile("model.uvl", "output.dimacs");

    if (success) {
        std::cout << "Conversion successful!" << std::endl;
    } else {
        std::cerr << "Conversion failed!" << std::endl;
        return 1;
    }

    // Conversion with Tseitin mode
    uvl2dimacs::ConversionOptions options;
    options.mode = uvl2dimacs::TransformationMode::Tseitin;
    options.verbose = true;

    success = converter.convertFile("model.uvl", "output_tseitin.dimacs", options);

    return success ? 0 : 1;
}
```

### DIMACS to Graphs Only

```cpp
#include <dimacs2graphs/Dimacs2GraphsAPI.hh>
#include <iostream>

int main() {
    // Create analyzer instance
    dimacs2graphs::Dimacs2GraphsAPI analyzer;

    // Configure analysis
    dimacs2graphs::AnalysisOptions options;
    options.num_threads = 4;
    options.output_dir = "./output";
    options.basename = "mymodel";

    // Run analysis
    dimacs2graphs::AnalysisResult result = analyzer.analyze("formula.dimacs", options);

    if (result.success) {
        std::cout << "Graph generation successful!" << std::endl;
        std::cout << "Core features: " << result.core_features.size() << std::endl;
        std::cout << "Dead features: " << result.dead_features.size() << std::endl;
        std::cout << "Requires edges: " << result.num_requires_edges << std::endl;
        std::cout << "Excludes edges: " << result.num_excludes_edges << std::endl;
    } else {
        std::cerr << "Analysis failed: " << result.error_message << std::endl;
        return 1;
    }

    return 0;
}
```

### Backbone Detection Only

```cpp
#include <backbone_solver/BackboneSolverAPI.hh>
#include <iostream>

int main() {
    // Create solver instance
    backbone_solver::BackboneSolverAPI solver;

    // Load DIMACS file
    if (!solver.loadDimacs("formula.dimacs")) {
        std::cerr << "Failed to load DIMACS file!" << std::endl;
        return 1;
    }

    // Compute backbone
    backbone_solver::BackboneResult result = solver.computeBackbone();

    if (result.success) {
        std::cout << "Backbone computation successful!" << std::endl;
        std::cout << "Positive backbone (core): " << result.positive_backbone.size() << std::endl;
        std::cout << "Negative backbone (dead): " << result.negative_backbone.size() << std::endl;

        // Print core features
        std::cout << "\nCore features:" << std::endl;
        for (int lit : result.positive_backbone) {
            std::cout << "  Variable " << lit << std::endl;
        }
    } else {
        std::cerr << "Backbone computation failed!" << std::endl;
        return 1;
    }

    return 0;
}
```

## Building and Running Examples

### Building API Examples

```bash
# Build all API examples
make api-examples

# The binaries will be in ./bin/
ls -l bin/*analysis
```

### Running Built Examples

```bash
# Simple analysis
./bin/simple_analysis examples/Car/model.uvl

# Advanced analysis with custom settings
./bin/advanced_analysis examples/Car/model.uvl ./output 4

# Batch processing
./bin/batch_analysis ./examples ./output 8
```

## Integration Examples

### Integration with Existing Build System

**CMakeLists.txt**:
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Find Strong4VM
find_library(STRONG4VM_API strong4vm_api PATHS ~/lib)
include_directories(~/include)

add_executable(my_analyzer my_analyzer.cc)
target_link_libraries(my_analyzer ${STRONG4VM_API})
```

### Python Integration (via subprocess)

```python
import subprocess
import json
import os

def analyze_model(uvl_file, output_dir, num_threads=4):
    """Run Strong4VM analysis from Python."""
    cmd = [
        "./bin/strong4vm",
        uvl_file,
        "-o", output_dir,
        "-t", str(num_threads)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode == 0:
        # Parse output files
        basename = os.path.splitext(os.path.basename(uvl_file))[0]
        core_file = os.path.join(output_dir, f"{basename}__core.txt")
        dead_file = os.path.join(output_dir, f"{basename}__dead.txt")

        with open(core_file) as f:
            core_features = [line.strip() for line in f if line.strip()]

        with open(dead_file) as f:
            dead_features = [line.strip() for line in f if line.strip()]

        return {
            "success": True,
            "core": core_features,
            "dead": dead_features
        }
    else:
        return {
            "success": False,
            "error": result.stderr
        }

# Usage
result = analyze_model("model.uvl", "./output", num_threads=8)
print(f"Core features: {len(result['core'])}")
print(f"Dead features: {len(result['dead'])}")
```

## Common Patterns

### Error Handling

```cpp
strong4vm::APIResult result = api.analyze(config);

if (!result.success) {
    if (result.error_message.find("UNSAT") != std::string::npos) {
        std::cerr << "Model has no valid configurations!" << std::endl;
    } else if (result.error_message.find("file not found") != std::string::npos) {
        std::cerr << "Input file does not exist!" << std::endl;
    } else {
        std::cerr << "Unknown error: " << result.error_message << std::endl;
    }
    return 1;
}
```

### Performance Monitoring

```cpp
auto start = std::chrono::high_resolution_clock::now();
strong4vm::APIResult result = api.analyze(config);
auto end = std::chrono::high_resolution_clock::now();

auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
std::cout << "Total execution time: " << duration.count() << " ms" << std::endl;
```

## See Also

- @ref getting_started for installation
- @ref architecture for system design
- @ref limitations for constraints
- API Reference: @ref strong4vm::Strong4VMAPI
