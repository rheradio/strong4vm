# Backbone Solver Component Overview

## Purpose

The Backbone Solver is a high-performance SAT backbone detection engine that identifies literals that have the same value in all satisfying assignments of a Boolean formula.

This component is the core analysis engine used by [Dimacs2Graphs](../../docs/html/index.html) in the [Strong4VM](../../../docs/html/index.html) pipeline.

## What is a Backbone?

### Definition

Given a satisfiable Boolean formula φ:

- **Positive Backbone**: Literals that are TRUE in all satisfying assignments
- **Negative Backbone**: Literals that are FALSE in all satisfying assignments

### Example

Formula: `(a ∨ b) ∧ c`

Satisfying assignments:
- a=T, b=T, c=T
- a=T, b=F, c=T
- a=F, b=T, c=T

**Analysis**:
- Variable `c` is always TRUE → Positive backbone
- No variable is always FALSE → No negative backbone
- Variables `a` and `b` vary → Not in backbone

### Applications

Backbones are useful for:
1. **Feature Models**: Identify mandatory (core) and forbidden (dead) features
2. **Configuration**: Find fixed choices across all valid configurations
3. **Optimization**: Simplify formulas by eliminating backbone variables
4. **Analysis**: Understand structural properties of Boolean formulas

## Detection Algorithms

The Backbone Solver provides two detection algorithms implementing different heuristic strategies.

### CheckCandidatesOneByOne (Default)

**Strategy**: Uses SAT solver activity bumping for improved performance.

**Algorithm**:
```
For each variable v:
    1. Bump v's activity in solver
    2. Test if φ ∧ v is SAT
    3. Test if φ ∧ ¬v is SAT
    4. If only one is SAT, v is backbone
```

**Characteristics**:
- **Activity Bumping**: Increases variable's decision priority
- **Faster**: Heuristic guides solver to relevant parts of search space
- **Default Choice**: Best performance in most cases

**When to Use**:
- Default for all use cases
- Large formulas where performance matters
- When backbone variables are scattered

### CheckCandidatesOneByOneWithoutAttention (Baseline)

**Strategy**: Tests variables without activity manipulation.

**Algorithm**:
```
For each variable v:
    1. Test if φ ∧ v is SAT (no activity bumping)
    2. Test if φ ∧ ¬v is SAT
    3. If only one is SAT, v is backbone
```

**Characteristics**:
- **No Heuristics**: Pure backbone detection
- **Simpler**: Easier to understand and reason about
- **Baseline**: Reference implementation for comparison

**When to Use**:
- Benchmarking and comparison
- Research purposes
- When activity bumping causes issues (rare)

## Architecture

### Template Method Pattern

```cpp
class BackBone {
protected:
    virtual void initializeDetector() = 0;
    virtual bool testLiteral(int lit) = 0;

public:
    BackboneResult computeBackbone() {
        initializeDetector();
        for (each variable v) {
            if (testLiteral(v))
                positive_backbone.insert(v);
            else if (testLiteral(-v))
                negative_backbone.insert(v);
        }
        return result;
    }
};
```

Concrete detectors implement specific strategies.

### Key Components

**BackboneSolverAPI**: High-level interface
```cpp
class BackboneSolverAPI {
public:
    bool loadDimacs(const std::string& file);
    BackboneResult computeBackbone(DetectorType type);
};
```

**BackBone**: Base class for detectors
- Defines algorithm skeleton
- Derived classes implement strategies

**MiniSat Integration**: IPASIR interface
- Standard incremental SAT solver interface
- MiniSat 2.2.0 as underlying solver

**LiteralSet**: Efficient data structure
- Compact storage for literal sets
- Fast membership testing

## Integration with Dimacs2Graphs

Dimacs2Graphs uses Backbone Solver to identify variable relationships:

```
DIMACS Formula
    ↓
BackboneSolverAPI::computeBackbone()
    ↓
Positive Backbone (Core Features)
Negative Backbone (Dead Features)
    ↓
Graph Generation (Requires/Excludes)
```

**Thread Safety**: Each Dimacs2Graphs thread gets its own Backbone Solver instance (pre-initialized).

## Performance

### Time Complexity

**Worst Case**: O(n × s) where:
- n = number of variables
- s = time per SAT query

**In Practice**:
- Activity bumping reduces s significantly
- Many variables short-circuit (backbone detected early)
- Incremental solving reuses learned clauses

### Optimization Techniques

1. **Incremental Solving**: Reuses solver state between queries
2. **Activity Bumping**: Guides solver to relevant decisions
3. **Learned Clauses**: Previous queries inform future queries
4. **Polarity Control**: Sets preferred initial assignments

## API Usage

### Basic Usage

```cpp
#include <backbone_solver/BackboneSolverAPI.hh>

backbone_solver::BackboneSolverAPI solver;

// Load formula
if (!solver.loadDimacs("formula.dimacs")) {
    std::cerr << "Failed to load file" << std::endl;
    return 1;
}

// Compute backbone
auto result = solver.computeBackbone();

if (result.success) {
    std::cout << "Positive backbone: " << result.positive_backbone.size() << std::endl;
    std::cout << "Negative backbone: " << result.negative_backbone.size() << std::endl;
}
```

### Selecting Detector Algorithm

```cpp
// Use default (with activity bumping)
auto result = solver.computeBackbone(backbone_solver::DetectorType::WithAttention);

// Use baseline (without activity bumping)
auto result = solver.computeBackbone(backbone_solver::DetectorType::WithoutAttention);
```

### Accessing Results

```cpp
auto result = solver.computeBackbone();

// Iterate over positive backbone
for (int var : result.positive_backbone) {
    std::cout << "Variable " << var << " is always true" << std::endl;
}

// Iterate over negative backbone
for (int var : result.negative_backbone) {
    std::cout << "Variable " << var << " is always false" << std::endl;
}
```

## Building

```bash
# From backbone_solver/src directory
make                # Build CLI (output: ../bin/backbone_solver)
make api            # Build API example
make CXX=clang++    # Use clang (recommended on macOS)
make CXX=g++        # Use g++ (recommended on Linux)
```

## Thread Safety

**CRITICAL**: Backbone Solver API is NOT thread-safe.

### Safe Usage Pattern

**Correct**:
```cpp
// Pre-initialize in main thread
std::vector<BackboneSolverAPI*> solvers(num_threads);
for (int i = 0; i < num_threads; i++) {
    solvers[i] = new BackboneSolverAPI();
    solvers[i]->loadDimacs("formula.dimacs");
}

// Use in parallel
#pragma omp parallel for
for (int task = 0; task < num_tasks; task++) {
    int tid = omp_get_thread_num();
    solvers[tid]->computeBackbone();
}
```

**Incorrect**:
```cpp
// ✗ NEVER initialize inside parallel region
#pragma omp parallel
{
    BackboneSolverAPI solver;  // RACE CONDITION!
    solver.computeBackbone();
}
```

## Limitations

1. **SAT Requirement**: Formula must be satisfiable
2. **Memory**: Each instance requires ~60-70 MB
3. **Thread Safety**: Not thread-safe, requires pre-initialization pattern

## Implementation Details

### MiniSat Interface

Uses IPASIR (Incremental Propositional Satisfiability) interface:
```cpp
void ipasir_add(int lit);      // Add literal to clause
int ipasir_solve();            // Solve formula
void ipasir_assume(int lit);   // Add assumption
```

### Learned Clause Management

- Solver retains learned clauses between queries
- Improves performance on related queries
- Reset only when necessary

### Activity Bumping Implementation

```cpp
// Bump variable activity before testing
void bumpActivity(int var) {
    solver->setPolarity(var, true);   // Prefer var=true
    solver->bumpVariableActivity(var); // Increase priority
}
```

## Extending the Backbone Solver

### Adding New Detectors

1. **Create Detector Class**:
```cpp
class MyDetector : public BackBone {
protected:
    void initializeDetector() override {
        // Custom initialization
    }

    bool testLiteral(int lit) override {
        // Custom testing strategy
        return ...;
    }
};
```

2. **Register in API**:
```cpp
// In BackboneSolverAPI::create_backbone_detector()
case DetectorType::MyDetector:
    return new MyDetector(...);
```

3. **Add to DetectorType Enum**:
```cpp
enum class DetectorType {
    WithAttention,
    WithoutAttention,
    MyDetector  // Add here
};
```

## Related Documentation

- [Dimacs2Graphs](../../docs/html/index.html) - Uses this component
- [Strong4VM Main Documentation](../../../docs/html/index.html)
- API Reference: @ref backbone_solver::BackboneSolverAPI
