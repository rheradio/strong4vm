# Strong4VM Architecture {#architecture}

This page provides a comprehensive overview of Strong4VM's architecture, design patterns, and component interactions.

## System Overview

![Architecture Diagram](../figures/architecture.png)

Strong4VM implements a three-stage pipeline for analyzing variability models:

```
UVL Feature Model  →  [UVL2Dimacs + BackboneSimplifier]  →  Simplified DIMACS CNF  →  [Dimacs2Graphs + BoneDigger]  →  Graphs
```

Each stage transforms the input data, ultimately producing strong transitive dependency and conflict graphs along with core and dead feature lists.

## Component Architecture

### 1. UVL2Dimacs: Feature Model to CNF Converter

**Purpose**: Converts Universal Variability Language (UVL) feature models into DIMACS CNF format suitable for SAT-based analysis.

**Architecture**: Multi-layer design with clear separation of concerns

```
User Input (UVL file)
    ↓
Parser Layer (ANTLR4-generated)
    ↓
AST Layer (Abstract Syntax Tree)
    ↓
Generator Layer (CNF Transformation)
    ↓
Writer Layer (DIMACS output)
    ↓
BackboneSimplifier (backbone_solver subprocess → simplified DIMACS)
```

**Key Classes**:
- `FeatureModel` - Represents the complete feature model structure
- `FMToCNF` - Orchestrates CNF transformation
- `RelationEncoder` - Encodes 5 UVL relation types (mandatory, optional, or, alternative, cardinality)
- `ASTNode` - Abstract syntax tree node representation
- `CNFModel` - Represents the CNF formula
- `DimacsWriter` - Outputs standard DIMACS format
- `BackboneSimplifier` - Calls `backbone_solver -r -a 5` as subprocess; removes backbone-satisfied clauses and adds unit clauses for backbone literals, reducing formula size by 30–50%

**Transformation Modes**:
1. **Straightforward Mode** (default)
   - Direct Negation Normal Form (NNF) + distribution law
   - Produces fewer variables
   - Better for simple constraints

2. **Tseitin Mode**
   - Introduces auxiliary variables for subexpressions
   - Produces 3-CNF formulas
   - Shorter clauses, more predictable structure

**Backbone Simplification** (optional, disabled by default in strong4vm):
- Enabled via `set_backbone_simplification(true)` on the `UVL2Dimacs` API object
- Invokes `backbone_solver -r -a 5 <file>` as a subprocess after DIMACS generation
- Identifies backbone literals (features that are core or dead in all configurations)
- Removes clauses satisfied by those literals; adds explicit unit clauses for them
- Reduces formula size by 30–50%
- The `backbone_solver` binary is expected at `<exe_dir>/../backbone_solver/bin/backbone_solver`

**Namespace**: `uvl2dimacs`

### 2. Dimacs2Graphs: CNF to Graph Generator

**Purpose**: Analyzes SAT formulas using backbone detection to find strong transitive relationships between variables.

**Architecture**: Multi-threaded parallel processing with backbone analysis

```
DIMACS CNF File
    ↓
DIMACS Reader (parse formula)
    ↓
Backbone Solver (parallel analysis)
    ↓
Graph Generator (requires/excludes relationships)
    ↓
Pajek Writer (graph output)
```

**Key Features**:
- **Parallel Processing**: Configurable thread count for performance
- **SAT-based Analysis**: Uses backbone detection to identify relationships
- **Graph Generation**: Produces both dependency and conflict graphs

**Output**:
- Requires graph (directed): v → i means selecting v requires selecting i
- Excludes graph (undirected): v — i means v and i are mutually exclusive
- Core features (always selected)
- Dead features (never selected)

**Namespace**: `dimacs2graphs`

### 3. BoneDigger: Backbone Detection Engine

**Purpose**: High-performance SAT backbone detection using MiniSat with three detection strategies. BoneDigger is shared between dimacs2graphs (via API) and uvl2dimacs (as a subprocess for backbone simplification).

**Location**: `uvl2dimacs/backbone_solver/`

**Architecture**: Template method pattern with detector strategies

```
DIMACS Formula
    ↓
BackBone Base Class (template method)
    ↓
Detector Strategy (algorithm selection)
    ↓
Extended MiniSat Interface
    ↓
Backbone Results (positive/negative literals)
```

**Detector Algorithms**:

1. **CheckCandidatesOneByOne** (`one`) — default, recommended
   - Upper-bound iterative elimination with activity bumping
   - Bumps variable activities to guide the solver toward relevant solutions

2. **FastOnCliffsSlowOnPlains** (`flatland`)
   - Adaptive strategy switching between rapid elimination (cliffs) and careful verification (plains)
   - Based on convergence rate detection

3. **RushAndPray** (`rush`)
   - Rush-and-verify approach: starts from an initial solution, iteratively refines candidates
   - Used by BackboneSimplifier for fast formula preprocessing

**Key Classes**:
- `BackBone` - Base class defining template method pattern
- `BoneDiggerAPI` - High-level PIMPL interface for backbone computation, includes `compute_backbone_with_assumptions()` for per-variable analysis in dimacs2graphs
- `LiteralSet` - Efficient data structure for literal management
- `DIMACSReader` - Parses DIMACS files

**Namespace**: `bonedigger`

## Design Patterns

### PIMPL Idiom (Strong4VM API)

The Strong4VMAPI uses the Pointer-to-Implementation (PIMPL) idiom for ABI stability:

```cpp
class Strong4VMAPI {
public:
    // Public interface
    APIResult analyze(...);
private:
    std::unique_ptr<Impl> pimpl;  // Implementation hidden
};
```

**Benefits**:
- Binary compatibility across versions
- Faster compilation (reduces header dependencies)
- Implementation details hidden from users

### Thread Safety Pattern

**Critical Design Requirement**: Backbone Solver API is NOT thread-safe.

**Pre-initialization Pattern** (used in Dimacs2Graphs):

```cpp
// CORRECT: Initialize solvers sequentially in main thread
std::vector<BoneDiggerAPI*> solvers;
for (int i = 0; i < num_threads; i++) {
    solvers.push_back(new BoneDiggerAPI());
}

// Then use solvers in parallel
#pragma omp parallel for
for (int var = 0; var < num_vars; var++) {
    int tid = omp_get_thread_num();
    solvers[tid]->analyze(...);  // Each thread gets its own solver
}
```

**Key Rules**:
1. Create all solver instances sequentially in the main thread
2. Never initialize solvers inside parallel regions
3. Each thread gets its own independent solver instance
4. Thread-local buffers for output

**Memory Requirements**: ~60-70 MB per thread

### Template Method Pattern (Backbone Detection)

The `BackBone` base class defines the detection algorithm skeleton:

```cpp
class BackBone {
protected:
    virtual void initializeDetector() = 0;
    virtual bool detectLiteral(int lit) = 0;
public:
    void computeBackbone() {
        initializeDetector();
        for (each literal) {
            if (detectLiteral(lit))
                addToBackbone(lit);
        }
    }
};
```

Concrete detectors implement specific strategies by overriding virtual methods.

## Data Flow

### Complete Pipeline Flow

1. **Input Stage**:
   - User provides UVL file or DIMACS file
   - CLI tool determines input type

2. **Conversion Stage** (if UVL input):
   - ANTLR4 parser generates AST
   - AST traversal builds `FeatureModel`
   - CNF transformation produces `CNFModel`
   - DIMACS writer outputs intermediate .dimacs file
   - *(Optional)* **Backbone simplification**: if `set_backbone_simplification(true)` is called on the `UVL2Dimacs` object, BoneDigger (RushAndPray) identifies backbone literals; clauses satisfied by them are removed and unit clauses are added, reducing formula size by 30–50%. Disabled by default in the strong4vm CLI and unified API.

3. **Analysis Stage**:
   - DIMACS reader parses formula
   - Formula validated for satisfiability
   - Solver instances created (one per thread)

4. **Backbone Detection**:
   - Parallel processing of variables
   - Each thread checks subset of variables
   - Results aggregated into backbone sets

5. **Graph Generation**:
   - Backbone results analyzed for relationships
   - Dependency graph built (requires)
   - Conflict graph built (excludes)
   - Pajek .net files written

6. **Output Stage**:
   - Write graph files
   - Write core/dead feature lists
   - Optional: keep intermediate DIMACS

### Variable Encoding

**UVL to DIMACS Mapping**:
- Each feature maps to a Boolean variable
- Comments preserve feature names: `c <var_number> <feature_name>`
- Positive literal: feature enabled
- Negative literal: feature disabled

**Relation Encoding** (5 types in UVL):
1. **Mandatory**: parent → child (if parent selected, child must be selected)
2. **Optional**: parent → (child or not child) (child selection is optional)
3. **Or**: parent → (at least one child selected)
4. **Alternative**: parent → (exactly one child selected)
5. **Cardinality**: parent → (n to m children selected)

Each relation type has specific CNF encoding rules implemented in `RelationEncoder`.

## Parallelization Strategy

### Thread Distribution

**Static Work Partitioning**:
- Variables divided into equal-sized ranges
- Each thread processes its assigned range
- No dynamic work stealing (simple and efficient)

**Thread Count Selection**:
```
optimal_threads = min(cpu_cores, num_variables / 50)
```

**Performance Characteristics**:
- Small formulas (<100 vars): 1-2 threads optimal
- Medium formulas (100-500 vars): 4-8 threads optimal
- Large formulas (>500 vars): scale to CPU core count

### Synchronization

- No locks during analysis (embarrassingly parallel)
- Each thread writes to thread-local buffers
- Final aggregation in main thread after parallel region

## File Organization

```
strong4vm/
├── cli/                    # Strong4VM unified CLI
│   └── strong4vm.cc        # Main entry point
├── api/                    # Strong4VM Unified API
│   ├── include/            # Public headers
│   ├── src/                # PIMPL implementation
│   └── examples/           # API usage examples
├── uvl2dimacs/             # UVL to DIMACS converter
│   ├── api/                # Conversion API
│   ├── generator/          # CNF transformation logic
│   ├── parser/             # ANTLR4 UVL parser
│   └── cli/                # Standalone CLI tool
├── dimacs2graphs/          # DIMACS to graphs generator
│   ├── api/                # Graph generation API
│   └── cli/                # Standalone CLI tool
├── backbone_solver/        # Runtime backbone solver binary
│   └── bin/                # backbone_solver executable (auto-built by `make`)
└── docs/                   # Documentation (this file!)
```

## Build System Architecture

**Hierarchical Makefiles**:
- Top-level Makefile orchestrates all components
- Component Makefiles handle specific builds
- CMake for UVL2Dimacs (ANTLR4 integration)
- Traditional Makefiles for other components

**Aggressive Optimization**:
- Compiler flags: `-O3 -march=native -flto`
- Link-time optimization for performance
- Native architecture tuning

## Component Interactions

### API Usage Example

```cpp
// Unified API usage
#include <strong4vm/Strong4VMAPI.hh>

Strong4VMAPI api;

// Configure analysis
AnalysisConfig config;
config.input_file = "model.uvl";
config.num_threads = 4;
config.output_dir = "./output";

// Run complete pipeline
APIResult result = api.analyze(config);

if (result.success) {
    std::cout << "Core features: " << result.num_core << std::endl;
    std::cout << "Dead features: " << result.num_dead << std::endl;
}
```

The API automatically:
1. Detects input type
2. Converts UVL to DIMACS if needed
3. Runs backbone analysis
4. Generates graphs
5. Writes output files

### Component Reuse

Each component can be used independently:

```cpp
// Use only UVL2Dimacs
UVL2DimacsAPI converter;
converter.convert("model.uvl", "output.dimacs");

// Use only Dimacs2Graphs
Dimacs2GraphsAPI analyzer;
analyzer.generateGraphs("formula.dimacs", "./output", 4);

// Use only BoneDigger backbone solver
BoneDiggerAPI solver;
solver.read_dimacs("formula.dimacs");
solver.create_backbone_detector("one");
auto backbone = solver.compute_backbone();
```

## Performance Considerations

### Bottlenecks

1. **Parsing** (UVL input): ANTLR4 parsing overhead
2. **CNF Transformation**: Complex formulas require many clauses
3. **SAT Solving**: Backbone detection requires multiple SAT queries
4. **I/O**: Writing large graph files

### Optimization Strategies

1. **Parallel Processing**: Use multiple threads for backbone detection
2. **Memory Pre-allocation**: Reduce dynamic allocations
3. **Cache Locality**: Thread-local data structures
4. **Activity Bumping**: SAT solver heuristics

## Extensibility Points

### Adding New UVL Features

Extend `RelationEncoder` to support new constraint types:
1. Add encoding method in `RelationEncoder.cc`
2. Update parser grammar if needed
3. Add unit tests

### Adding New Backbone Detectors

Implement the `BackBone` interface:
1. Inherit from `BackBone` base class
2. Implement pure virtual methods
3. Register in `BoneDiggerAPI::create_backbone_detector()`

### Adding New Output Formats

Extend graph writers:
1. Implement new writer class
2. Add format option to API
3. Update CLI argument parsing

## Related Documentation

- See @ref mainpage for overview
- See @ref getting_started for building and usage
- See @ref examples for code examples
- See @ref limitations for known constraints
