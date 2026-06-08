# Strong4VM

![](figures/icon.png)

*A tool for extracting Strong Transitive Dependency and Conflict Graphs from Variability Models*

## 📋 Overview

*Variability Models* are commonly used to represent the configuration space of software systems. These models specify the available configurable options or *features*, along with the constraints that govern these features (e.g., if feature *A* is enabled in a configuration, then feature *B* must also be enabled, while feature *C* must be disabled).

However, these constraints result in numerous additional indirect relationships due to their chaining. Reasoning about these relationships is extremely challenging in all but the most trivial variability models.

**Strong4VM** identifies all transitive strong relationships between features and represents them as graphs. The term *transitive* refers to relationships that can be formed by linking other relationships defined in the variability model, while *strong* indicates that these relationships are valid in all configurations that comply with the variability model.

**✨ Key Features:**

-   🎯 Unified interface: Single tool for UVL or DIMACS inputs. The [Universal Variability Language](https://universal-variability-language.github.io/) (UVL) is the standard notation for variability models; however, Strong4VM can also generate graphs for the Boolean translations of any variability model notation. This is made possible by its support for DIMACS files. For example, it can generate graphs using [KconfigReader](https://github.com/ckaestne/kconfigreader) first to convert Kconfig files (used in Linux, BusyBox, axTLS, and more) into Boolean formulas.

-   ⚡ High performance: Multi-threaded graph generation with configurable parallelism

-   📈 Comprehensive: Generates requires/excludes graphs and core/dead feature lists

-   ✅ Well-tested: Validated against [5,709 real-world variability models](https://doi.org/10.5281/zenodo.17277788).

## 🚀 Quick Start

### 💾 Installation

Strong4VM runs natively on **Linux** and **macOS**. On **Windows**, use the [Docker image](#-docker) instead — no native Windows build is supported.

``` bash
# Navigate to the strong4vm
cd strong4vm

# Build the tool
make

# Optional: Install to ~/bin
make install
```

### 📁 Output Files

For input `model.uvl` or `model.dimacs`, Strong4VM generates:

-   `model__requires.net` - Strong transitive dependency graph (a directed graph in [Pajek](http://mrvar.fdv.uni-lj.si/pajek/DrawEPS.htm) format)

-   `model__excludes.net` - Strong transitive conflict graph (an undirected graph in [Pajek](http://mrvar.fdv.uni-lj.si/pajek/DrawEPS.htm) format)

-   `model__core.txt` - Core features (enabled in all configurations)

-   `model__dead.txt` - Dead features (disabled in all configurations)

## 🏗️ Building

### 📦 Prerequisites

-   **C++17** compatible compiler (g++ 7+ or clang++ 5+)
-   **CMake** 3.10+
-   **Make**
-   **zlib** (for MiniSat)

### 🔨 Building Strong4VM (Unified CLI)

From the repository root:

``` bash
make                  # Build strong4vm (builds all dependencies automatically)
make install          # Install to ~/bin
make clean            # Clean build artifacts
make clean-all        # Deep clean (including all components)
make help             # Show all available targets
```

Output: `bin/strong4vm`

## 🐳 Docker

Strong4VM does not support native Windows builds. Docker is the recommended way to run it on Windows, and also works on Linux and macOS if you prefer not to install a local toolchain.

### Prerequisites

-   **Linux / macOS**: Docker Engine must be installed and the daemon running.
-   **Windows**: [Docker Desktop](https://www.docker.com/products/docker-desktop/) must be installed and **running** before any `docker` command will work. Start it from the Start menu and wait until the whale icon in the system tray shows *"Docker Desktop is running"*.

### Building the Image

``` bash
docker build -t strong4vm .
```

The image uses a two-stage build on Ubuntu 24.04: a builder stage that compiles all dependencies from scratch (ANTLR4 runtime, MiniSat, BoneDigger, strong4vm), and a slim runtime stage that contains only the resulting binaries. The first build takes around 5–10 minutes; subsequent builds are fast thanks to Docker layer caching.

### Running

The container's working directory is `/data`. The key idea is to **mount the folder that holds your model files** into that directory with `-v`, so the container can read your inputs and write its outputs back to your machine.

Say you want to analyze the BusyBox feature model included in the `examples/` folder. Navigate to `examples/` and run the command for your shell:

**Linux / macOS (bash/zsh)**
``` bash
cd examples
docker run --rm -v $(pwd):/data strong4vm busybox_01.uvl
```

**Windows — Command Prompt**
``` cmd
cd examples
docker run --rm -v %cd%:/data strong4vm busybox_01.uvl
```

**Windows — PowerShell**
``` powershell
cd examples
$dir = $PWD.Path.Replace('\', '/')
docker run --rm -v "${dir}:/data" strong4vm busybox_01.uvl
```

Strong4VM will read `busybox_01.uvl` from your local folder and write four files right next to it:

```
examples/
├── busybox_01__requires.net   ← strong transitive dependency graph
├── busybox_01__excludes.net   ← strong transitive conflict graph
├── busybox_01__core.txt       ← features enabled in every valid configuration
└── busybox_01__dead.txt       ← features disabled in every valid configuration
```

To send output to a separate folder, or to speed up analysis with multiple threads, add `-o` and `-t`:

**Linux / macOS**
``` bash
docker run --rm -v $(pwd):/data strong4vm busybox_01.uvl -t 4 -o /data/results
```

**Windows — Command Prompt**
``` cmd
docker run --rm -v %cd%:/data strong4vm busybox_01.uvl -t 4 -o /data/results
```

**Windows — PowerShell**
``` powershell
$dir = $PWD.Path.Replace('\', '/')
docker run --rm -v "${dir}:/data" strong4vm busybox_01.uvl -t 4 -o /data/results
```

DIMACS files work exactly the same way — just pass the `.dimacs` file instead of a `.uvl` file.

## 📖 Usage

### 🎯 CLI

``` bash
./bin/strong4vm model.uvl                    # Analyze a UVL feature model
./bin/strong4vm formula.dimacs               # Analyze a DIMACS formula
./bin/strong4vm model.uvl -t 8               # Use 8 threads
./bin/strong4vm model.uvl -o ./output -k     # Custom output dir + keep DIMACS
./bin/strong4vm model.uvl -e                 # Enable Tseitin transformation
```

**⚙️ Options:**

-   `-t, --threads N` - Number of threads for graph generation (default: 1)
-   `-o, --output DIR` - Output directory (default: same directory as input file)
-   `-k, --keep-dimacs` - Keep intermediate DIMACS file (UVL input only)
-   `-e, --enable-tseitin` - Enable Tseitin transformation for cross-tree constraints (see [uvl2dimacs Architecture](#-uvl2dimacs-architecture))
-   `-h, --help` - Display help message

### 🔗 API

**See [API Documentation](docs/html/index.html) for usage details.**

Build API examples:

``` bash
make api-examples
```

The resulting binaries will be generated in the `./bin` folder.

## 🏛️ Architecture

### 🔍 Architecture Overview

As the following figure shows, Strong4VM consists of three main components: two compilers, one that translates UVL to DIMACS and another that converts DIMACS to Graphs, and a Backbone solver that detects all transitive strong dependencies and conflicts represented in the graphs.

![](figures/architecture.png)

### 🔄 uvl2dimacs Architecture

Multi-layer design for converting UVL feature models to CNF format using ANTLR4 parser. Supports two conversion modes: **Straightforward** (default, direct conversion, fewer variables, potentially longer clauses) and **Tseitin** (introduces auxiliary variables for cross-tree constraint expressions to prevent exponential clause growth; feature tree relations are always encoded directly since they are already valid CNF disjunctions). An optional **backbone simplification** pass is available via the API (`set_backbone_simplification(true)`): BoneDigger identifies backbone literals and simplifies the DIMACS formula before graph analysis, reducing formula size by 30–50%. This pass is disabled by default in the strong4vm CLI and unified API.

**For detailed architecture, see [Doxygen Documentation](docs/html/group__UVL2Dimacs.html)**

### 📊 dimacs2graphs Architecture

Analyzes SAT formulas using backbone detection to generate dependency graphs. Supports multi-threaded parallel processing. Auxiliary variables (`aux_*` and `k!\d+`) are automatically excluded from backbone iteration and graph output.

**For detailed architecture, see [Doxygen Documentation](docs/html/group__ParallelGraphs.html)**

### 🧠 Backbone Solver Architecture (BoneDigger)

High-performance backbone detection engine (BoneDigger) using MiniSat with three detection strategies: **CheckCandidatesOneByOne** (reliable, activity-bumping), **FastOnCliffsSlowOnPlains** (adaptive cliff/plain), and **RushAndPray** (fast rush-and-verify). BoneDigger is also available for optional backbone simplification in uvl2dimacs (disabled by default in the strong4vm pipeline).

**For detailed architecture, see [Doxygen Documentation](docs/html/group__API.html)**

## 📄 Input Formats

### 📝 UVL Format

Universal Variability Language feature model format. See [UVL specification](https://universal-variability-language.github.io/).

### 🔢 DIMACS CNF Format

Standard SAT solver input format:

```         
p cnf <variables> <clauses>
<literal1> <literal2> ... 0
<literal1> <literal2> ... 0
...
```

-   Lines starting with `c` are comments
-   For dimacs2graphs, comments like `c <var_number> <feature_name>` map variables to feature names
-   Variables whose names start with `aux_` or match `k!\d+` are treated as Tseitin auxiliary variables and automatically excluded from the graph output. The `k!\d+` pattern covers the naming convention used by [KconfigReader](https://github.com/ckaestne/kconfigreader)

## 📚 Documentation

### 📖 Generating Documentation

Generate comprehensive Doxygen documentation:

``` bash
# Unified project documentation
make docs
# Output: docs/html/index.html
```

**Requirements:** Doxygen 1.9.0+, Graphviz (optional, for diagrams)

### 📁 Project Structure

```         
strong4vm/
├── bin/                    # Built executables
├── cli/                    # Strong4VM CLI source
├── api/                    # Strong4VM Unified API
│   ├── include/            # Public headers
│   ├── src/                # Implementation
│   ├── examples/           # Usage examples
│   └── docs/               # API documentation
├── uvl2dimacs/             # UVL to DIMACS converter + BoneDigger backbone solver
│   ├── cli/                # CLI tool
│   ├── api/                # C++ API (include/, src/, examples/)
│   ├── generator/          # Core conversion logic (BackboneSimplifier, FMToCNF, …)
│   ├── parser/             # ANTLR4 UVL parser
│   ├── backbone_solver/    # BoneDigger backbone detection engine
│   │   └── src/
│   │       ├── api/        # BoneDiggerAPI
│   │       ├── detectors/  # CheckCandidatesOneByOne, FastOnCliffsSlowOnPlains, RushAndPray
│   │       ├── data_structures/ # LiteralSet
│   │       ├── io/         # DIMACS file reading
│   │       ├── minisat/    # MiniSat SAT solver
│   │       └── minisat_interface/ # Extended MiniSat wrapper
│   ├── build/              # CMake build artifacts (generated)
│   ├── docs/               # Component documentation
│   └── third_party/        # ANTLR4 runtime source and build artifacts
├── dimacs2graphs/          # DIMACS to graphs generator
│   ├── cli/                # CLI tool
│   ├── api/                # C++ API
│   ├── bin/                # Component executables (generated)
│   └── docs/               # Component documentation
├── backbone_solver/        # backbone_solver runtime binary (generated by make)
│   └── bin/                # Used by uvl2dimacs BackboneSimplifier as a subprocess
├── examples/               # Sample models: 45+ UVL files + selected DIMACS files
├── figures/                # Architecture diagrams (architecture.png, icon.svg)
├── docs/                   # Doxygen documentation source and generated HTML
├── Dockerfile              # Multi-stage Docker image (Ubuntu 24.04)
├── .gitignore              # Excludes build artifacts and generated files
├── Makefile                # Build system orchestration
└── README.md               # This file
```

## ⚠️ Limitations

1.  **Feature Cardinality:** UVL `cardinality [1..*]` notation not expanded (requires indexed feature generation)
2.  **Arithmetic Constraints:** Comparison and arithmetic operators filtered out (requires SMT solver, not pure SAT)

## 🔧 Troubleshooting

### 🛠️ Build Issues

**Clean and rebuild:**

``` bash
make clean-all
make
```

**Missing dependencies:**

``` bash
# Linux (Ubuntu/Debian)
sudo apt-get install build-essential cmake zlib1g-dev

# macOS (Xcode CLT provides the compiler and make; Homebrew provides cmake)
xcode-select --install
brew install cmake
```

## 📖 Citation

If you use Strong4VM in your research, please cite:

```bibtex
@article{heradio2026strong4vm,
  title   = {{Strong4VM: From variability models to strong transitive dependency and conflict graphs}},
  journal = {SoftwareX},
  volume  = {34},
  pages   = {102663},
  year    = {2026},
  doi     = {https://doi.org/10.1016/j.softx.2026.102663},
  author  = {Ruben Heradio and Luis Cambelo and Miguel A. Olivero and Jose M. Sanchez
             and Alberto {Perez Garcia-Plaza} and David Fernandez-Amoros},
  keywords = {Variability models, Strong relationships, SAT solving, Backbone detection,
              Software product lines, Network analysis},
  abstract = {This paper introduces Strong4VM, a C++ tool that extracts complete graphs
              of strong transitive dependencies and conflicts from variability models using
              multi-threaded SAT-based backbone detection. The resulting graphs, exported
              in standard Pajek format, enable structural analysis with network analysis
              tools such as Pajek, igraph, Neo4j, and Gephi. Strong4VM has been validated
              on 5709 real-world models (within a range of 99-35,907 features), generating
              a total of 3.2⋅10⁷ nodes, 4.8⋅10⁸ require arcs, and 3.1⋅10⁸ exclude edges.
              The analysis of the synthesized graphs reveals domain-specific architectural
              patterns that provide practitioners with diagnostic indicators for quality
              assessment and evolution planning. The tool and the graph dataset are publicly
              available for reproducible research.}
}
```

## 📜 License, Authors & Funding

### ⚖️ Licensing

Strong4VM has a MIT License, and is built upon: 

-   **MiniSat**: MIT License
-   **ANTLR4**: BSD License

### 👥 Authors

-   Ruben Heradio, [rheradio\@issi.uned.es](mailto:rheradio@issi.uned.es)
-   Luis Cambelo, [lcambelo1\@alumno.uned.es](mailto:lcambelo1@alumno.uned.es)
-   Miguel A. Olivero, [molivero\@us.es](mailto:molivero@us.es)
-   José Manuel Sánchez Ruíz, [jsanchez7\@us.es](mailto:jsanchez7@us.es)
-   Alberto Pérez García-Plaza, [alberto.perez\@lsi.uned.es](mailto:alberto.perez@lsi.uned.es)
-   David Fernández Amorós, [david\@issi.uned.es](mailto:david@issi.uned.es)

### 💰 Funding

This work is funded by FEDER/Spanish Ministry of Science, Innovation and Universities (MCIN)/Agencia Estatal de Investigacion (AEI) under project COSY (PID2022-142043NB-I00).
