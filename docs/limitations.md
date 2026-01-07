# Known Limitations and Troubleshooting {#limitations}

This page documents known limitations, constraints, and solutions to common issues in Strong4VM.

## UVL Conversion Limitations

### Feature Cardinality

**Limitation**: UVL cardinality notation `[1..*]` is not fully expanded.

**Details**: Feature cardinality with unbounded upper limits requires indexed feature generation (creating separate variables for each instance), which is not currently implemented.

**Supported**:
```
features
    required_feature [1..3]  ✓ Supported (bounded)
```

**Not Supported**:
```
features
    optional_feature [1..*]  ✗ Not supported (unbounded)
```

**Workaround**: Manually specify an upper bound if possible, or reformulate the constraint.

### Arithmetic Constraints

**Limitation**: Comparison and arithmetic operators in constraints are filtered out during conversion.

**Details**: Arithmetic constraints require SMT (Satisfiability Modulo Theories) solvers, not pure SAT solvers. Strong4VM uses SAT-based analysis and cannot handle arithmetic.

**Not Supported**:
```
constraints
    featureA.value + featureB.value < 10  ✗ Filtered out
    featureC.price >= 100  ✗ Filtered out
```

**Workaround**:
1. Abstract arithmetic constraints into Boolean relationships when possible
2. Use an SMT solver for models with essential arithmetic constraints
3. Pre-process the model to resolve arithmetic constraints

### CNF Transformation

**No Clause Minimization**: The generated CNF formulas are not minimized. This means:
- Some redundant clauses may be present
- Formula size may be larger than optimal
- Analysis may be slower than with optimized formulas

**No Subsumption Checking**: Clause subsumption is not performed, which could reduce formula size.

## Graph Generation Limitations

### Satisfiability Requirement

**Limitation**: Formulas must be satisfiable (SAT). Unsatisfiable (UNSAT) formulas are not supported.

**Details**: If your feature model has contradictory constraints with no valid configuration, Strong4VM will report an error.

**Error Message**:
```
Error: Formula is UNSAT (no valid configurations exist)
```

**Solution**:
1. Review your feature model's constraints for contradictions
2. Use a SAT solver to identify the UNSAT core
3. Remove or relax conflicting constraints

### Thread Count Limits

**Limitation**: Thread count is limited to the number of CPU cores.

**Details**: The tool validates thread count against available CPU cores and will fail if you request more threads than available.

**Error Handling**:
```bash
# If you have 8 cores but request 16 threads:
./bin/strong4vm model.uvl -t 16
# Error: Requested 16 threads but only 8 cores available
```

**Best Practices**:
- Use `nproc` (Linux) or `sysctl -n hw.ncpu` (macOS) to check core count
- Leave 1-2 cores free for system processes
- Optimal thread count: `min(cores - 1, num_variables / 50)`

### Memory Requirements

**Limitation**: Each thread requires approximately 60-70 MB of RAM.

**Details**: Parallel processing creates independent solver instances for each thread, each requiring its own memory allocation.

**Memory Estimation**:
```
Required RAM ≈ 60 MB × num_threads + 100 MB (base overhead)
```

**Examples**:
- 1 thread: ~160 MB
- 4 threads: ~340 MB
- 8 threads: ~580 MB
- 16 threads: ~1060 MB

**Solution for Memory-Constrained Systems**:
```bash
# Reduce thread count
./bin/strong4vm model.uvl -t 2
```

## Performance Limitations

### Small Formula Overhead

**Issue**: For very small formulas (<50 variables), multi-threading adds overhead.

**Explanation**: Thread creation and synchronization overhead exceeds the benefit of parallel processing.

**Recommendation**:
```bash
# For small models, use 1 thread
./bin/strong4vm small_model.uvl -t 1
```

### Large Formula Scalability

**Issue**: Very large formulas (>10,000 variables) may have long processing times.

**Factors**:
- SAT solving complexity (backbone detection requires multiple SAT queries)
- Graph generation requires checking all variable pairs
- Memory bandwidth limitations

**Optimization Strategies**:
1. Use maximum available threads
2. Ensure sufficient RAM
3. Use SSD storage for faster I/O
4. Consider formula simplification if possible

## Thread Safety Limitations

### Backbone Solver API

**Critical Limitation**: `BackboneSolverAPI` is NOT thread-safe.

**Correct Usage** (Pre-initialization Pattern):
```cpp
// ✓ CORRECT: Initialize in main thread
std::vector<BackboneSolverAPI*> solvers;
for (int i = 0; i < num_threads; i++) {
    solvers.push_back(new BackboneSolverAPI());
}

// Then use in parallel
#pragma omp parallel for
for (int var = 0; var < num_vars; var++) {
    int tid = omp_get_thread_num();
    solvers[tid]->analyze(...);
}
```

**Incorrect Usage**:
```cpp
// ✗ WRONG: Never initialize inside parallel region
#pragma omp parallel
{
    BackboneSolverAPI solver;  // RACE CONDITION!
    solver.analyze(...);
}
```

**Why This Matters**: Violating this pattern leads to:
- Race conditions
- Memory corruption
- Unpredictable crashes
- Incorrect results

## File Format Limitations

### UVL Parser

**Supported**: Standard UVL 2.0 syntax
**Not Supported**:
- Custom UVL extensions
- Non-standard comment formats
- Some edge cases in grammar

### DIMACS Format

**Requirements**:
- Standard DIMACS CNF format
- Header: `p cnf <vars> <clauses>`
- Clauses terminated with 0
- Comments start with 'c'

**Optional Feature Mapping**:
```
c <variable_number> <feature_name>
```

If feature name comments are missing, output will use variable numbers instead of feature names.

## Output Format Limitations

### Pajek .net Format

**Limitation**: Output is only in Pajek .net format.

**Workaround**: Convert to other formats using tools like:
- NetworkX (Python)
- Gephi (import and export)
- igraph (R/Python)

**Example NetworkX Conversion**:
```python
import networkx as nx

# Read Pajek file
G = nx.read_pajek("model__requires.net")

# Export to other formats
nx.write_gml(G, "model.gml")
nx.write_graphml(G, "model.graphml")
nx.write_edgelist(G, "model.edgelist")
```

## Troubleshooting

### Build Issues

#### CMake Errors

**Problem**: CMake configuration fails

**Common Causes**:
- CMake version too old (need 3.10+)
- Missing dependencies

**Solution**:
```bash
# Check CMake version
cmake --version

# Upgrade if needed (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install cmake

# Clean and rebuild
make clean-all
make
```

#### Compiler Errors

**Problem**: Compilation fails with C++ errors

**Common Causes**:
- Compiler doesn't support C++17
- Compiler too old

**Solution**:
```bash
# Check compiler version
g++ --version    # Need g++ 7+
clang++ --version  # Need clang++ 5+

# Upgrade compiler (Ubuntu/Debian)
sudo apt-get install g++-9

# Specify compiler
make CXX=g++-9
```

#### ANTLR4 Build Failures

**Problem**: ANTLR4 runtime fails to build

**Solution**:
```bash
# Clean ANTLR4 build
cd uvl2dimacs
make clean-all

# Rebuild from scratch
make all
```

### Runtime Issues

#### UNSAT Formula Error

**Problem**: "Formula is UNSAT" error

**Diagnosis**:
```bash
# Test with a SAT solver
minisat model.dimacs result.txt
# If UNSAT, your model has no valid configurations
```

**Solutions**:
1. Check for contradictory constraints
2. Review mandatory and alternative groups
3. Verify cross-tree constraints
4. Use a minimal example to isolate the issue

#### Out of Memory

**Problem**: Program crashes or system becomes unresponsive

**Diagnosis**:
- Check available RAM: `free -h` (Linux) or `vm_stat` (macOS)
- Estimate required RAM: 60 MB × threads + 100 MB

**Solutions**:
```bash
# Reduce thread count
./bin/strong4vm model.uvl -t 2

# Close other applications
# Upgrade system RAM if consistently hitting limits
```

#### Slow Performance

**Problem**: Analysis takes too long

**Diagnosis**:
- Check formula size: count variables and clauses in DIMACS file
- Monitor CPU usage: `top` or `htop`

**Optimization**:
```bash
# Use more threads (up to core count)
./bin/strong4vm model.uvl -t 8

# Profile to identify bottleneck
time ./bin/strong4vm model.uvl -t 4
```

#### File Not Found Errors

**Problem**: Cannot find input file or dependencies

**Solution**:
```bash
# Use absolute paths
./bin/strong4vm /absolute/path/to/model.uvl

# Or verify relative paths
ls -l model.uvl

# Check working directory
pwd
```

#### Permission Denied

**Problem**: Cannot write output files

**Solution**:
```bash
# Check directory permissions
ls -ld output_directory

# Create output directory
mkdir -p ./output

# Fix permissions
chmod 755 ./output
```

### WSL-Specific Issues

#### File System Synchronization

**Problem**: Files created but not visible or build artifacts missing

**Details**: Windows Subsystem for Linux may have file system sync issues

**Solution**:
```bash
# Run sync command
sync

# Use WSL filesystem (/home) instead of Windows paths (/mnt/c)
cd ~/strong4vm
make
```

## Performance Tuning

### Thread Count Selection

**Formula Size**:
- < 100 variables: 1-2 threads
- 100-500 variables: 4-8 threads
- 500-1000 variables: 8-12 threads
- \> 1000 variables: up to CPU core count

**Rule of Thumb**:
```
optimal_threads = min(cpu_cores - 1, num_variables / 50)
```

### Memory Optimization

For memory-constrained systems:
1. Use fewer threads
2. Process models sequentially in batch jobs
3. Close unnecessary applications
4. Consider cloud instances with more RAM

### I/O Optimization

For large batch processing:
1. Use SSD storage
2. Keep output on same filesystem as executable
3. Disable keepdimacs (-k) flag if not needed

## Getting Help

If you encounter issues not covered here:

1. **Check existing issues**: Visit the [GitHub repository](https://github.com/rheradio/Strong4VM/issues)
2. **Enable verbose output**: Add debug flags if available
3. **Create minimal example**: Isolate the problem with smallest possible model
4. **Report issue**: Include:
   - Strong4VM version
   - Operating system and version
   - Compiler version
   - Complete error message
   - Minimal reproducible example

## See Also

- @ref getting_started for installation and setup
- @ref examples for usage patterns
- @ref architecture for system design
- [README](../README.md) for quick reference
