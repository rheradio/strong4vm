# Dimacs2Graphs Component Overview

## Purpose

Dimacs2Graphs generates strong transitive dependency and conflict graphs from DIMACS CNF formulas using SAT-based backbone detection.

This component is the second stage of the [Strong4VM](../../docs/html/index.html) pipeline.

## Overview

Dimacs2Graphs analyzes propositional formulas to identify relationships between variables:

- **Dependency Relationships** (Requires): If variable v is true, then variable i must also be true
- **Conflict Relationships** (Excludes): Variables v and i cannot both be true simultaneously
- **Core Features**: Variables that are true in all satisfying assignments (positive backbone)
- **Dead Features**: Variables that are false in all satisfying assignments (negative backbone)

## Algorithm: Parallel Backbone Analysis

### High-Level Flow

```
DIMACS Formula
    ↓
Parse and Validate (must be SAT)
    ↓
Initialize Solver Instances (one per thread)
    ↓
Parallel Backbone Detection
    ├─→ Thread 1: Analyze variables 1...n/t
    ├─→ Thread 2: Analyze variables n/t+1...2n/t
    └─→ Thread t: Analyze variables ...n
    ↓
Aggregate Results
    ↓
Generate Graphs
    ├─→ Requires Graph (directed)
    └─→ Excludes Graph (undirected)
    ↓
Write Output Files
```

### Backbone Detection

For each variable v:

1. **Check if v is backbone** (always true or always false):
   - Test if formula ∧ v is SAT
   - Test if formula ∧ ¬v is SAT
   - If only one is SAT, v is backbone

2. **Find dependencies** (if v not backbone):
   - For each other variable i:
     - If formula ∧ v ∧ ¬i is UNSAT, then v requires i
     - If formula ∧ v ∧ i is UNSAT, then v excludes i

### Parallelization Strategy

**Static Work Partitioning**:
- Variables divided into equal ranges
- Each thread processes its range independently
- No synchronization needed during analysis

**Thread Count**:
```
optimal_threads = min(cpu_cores, num_variables / 50)
```

## Thread Safety Pattern

**Critical Rule**: Backbone Solver API is NOT thread-safe.

### Pre-Initialization Pattern (Required)

```cpp
// STEP 1: Initialize solvers SEQUENTIALLY in main thread
std::vector<BackboneSolverAPI*> solvers;
for (int t = 0; t < num_threads; t++) {
    solvers.push_back(new BackboneSolverAPI());
    solvers[t]->loadDimacs(dimacs_file);  // Load in main thread
}

// STEP 2: Use solvers in parallel (each thread gets its own instance)
#pragma omp parallel for
for (int var = 0; var < num_vars; var++) {
    int tid = omp_get_thread_num();
    // Each thread uses its pre-initialized solver
    solvers[tid]->checkVariable(var);
}

// STEP 3: Cleanup
for (auto* solver : solvers) {
    delete solver;
}
```

### Why Pre-Initialization Matters

**Correct** (Pre-initialize):
```cpp
// Main thread
for (int i = 0; i < threads; i++) {
    solvers[i] = new BackboneSolverAPI();  // ✓ Safe
}

// Parallel region
#pragma omp parallel
{
    int tid = omp_get_thread_num();
    solvers[tid]->analyze(...);  // ✓ Safe (already initialized)
}
```

**Incorrect** (Initialize in parallel):
```cpp
// ✗ WRONG - Race condition!
#pragma omp parallel
{
    BackboneSolverAPI solver;  // ✗ Concurrent initialization
    solver.analyze(...);
}
```

## Output Files

### Requires Graph (Dependency)

**Format**: Pajek .net (directed graph)

**File**: `<basename>__requires.net`

**Interpretation**: Edge v → i means "selecting feature v requires selecting feature i"

**Properties**:
- Directed graph
- Transitive: If v → i and i → j, then implicitly v → j
- Strong: Valid in ALL configurations

### Excludes Graph (Conflict)

**Format**: Pajek .net (undirected graph)

**File**: `<basename>__excludes.net`

**Interpretation**: Edge v — i means "features v and i are mutually exclusive"

**Properties**:
- Undirected graph
- Symmetric: If v excludes i, then i excludes v
- Strong: Conflict holds in ALL configurations

### Core Features List

**File**: `<basename>__core.txt`

**Content**: One feature per line

**Interpretation**: Features that must be selected in every valid configuration (positive backbone)

### Dead Features List

**File**: `<basename>__dead.txt`

**Content**: One feature per line

**Interpretation**: Features that must NOT be selected in any valid configuration (negative backbone)

## Performance Tuning

### Thread Count Selection

**Formula Size vs. Threads**:
- < 100 variables: 1-2 threads (threading overhead > benefit)
- 100-500 variables: 4-8 threads
- 500-1000 variables: 8-12 threads
- \> 1000 variables: up to CPU core count

**Rule of Thumb**:
```
threads = min(cpu_cores - 1, max(1, num_variables / 50))
```

### Memory Requirements

Each thread requires approximately 60-70 MB of RAM:

```
Required RAM ≈ num_threads × 65 MB + 100 MB (base)
```

**Examples**:
- 4 threads: ~360 MB
- 8 threads: ~620 MB
- 16 threads: ~1140 MB

### Performance Characteristics

**Time Complexity** (worst case):
- O(n²) where n = number of variables
- Each variable checked against all others

**In Practice**:
- Many checks short-circuit (backbone detection)
- Parallel processing provides near-linear speedup
- SAT solver heuristics significantly improve performance

## Integration with Strong4VM

Dimacs2Graphs is the analysis engine in the Strong4VM pipeline:

```
UVL → [UVL2Dimacs] → DIMACS → [Dimacs2Graphs + Backbone Solver] → Graphs
```

Can be used standalone:
```bash
# Direct DIMACS analysis
./bin/dimacs2graphs formula.dimacs ./output 4
```

## API Usage

### Basic Analysis

```cpp
#include <dimacs2graphs/Dimacs2GraphsAPI.hh>

dimacs2graphs::Dimacs2GraphsAPI analyzer;

dimacs2graphs::AnalysisOptions options;
options.num_threads = 4;
options.output_dir = "./output";

auto result = analyzer.analyze("formula.dimacs", options);

if (result.success) {
    std::cout << "Core: " << result.core_features.size() << std::endl;
    std::cout << "Dead: " << result.dead_features.size() << std::endl;
}
```

### Advanced Usage

```cpp
// Custom basename for output files
options.basename = "my_analysis";

// Enable verbose output
options.verbose = true;

// Run analysis
auto result = analyzer.analyze("formula.dimacs", options);

// Access detailed results
for (const auto& feature : result.core_features) {
    std::cout << "Core: " << feature << std::endl;
}
```

## Building

```bash
# From dimacs2graphs directory
make                # Build CLI and API
make cli            # Build CLI only
make clean          # Clean build artifacts

# Generate documentation
make docs
```

## Testing

```bash
# From dimacs2graphs/test directory
./run_tests.sh         # Run all tests
./run_tests.sh -q      # Quick mode (first 10 tests)
./run_tests.sh -v      # Verbose with diffs
./run_tests.sh -s      # Stop on first failure
```

## Limitations

1. **SAT Requirement**: Formula must be satisfiable (UNSAT not supported)
2. **Thread Limit**: Cannot exceed CPU core count
3. **Memory**: ~65 MB per thread required
4. **Thread Safety**: Must use pre-initialization pattern

See [Strong4VM Limitations](../../docs/html/limitations.html) for details.

## Related Documentation

- [Strong4VM Main Documentation](../../docs/html/index.html)
- [Backbone Solver](../backbone_solver/docs/html/index.html)
- API Reference: @ref dimacs2graphs::Dimacs2GraphsAPI
