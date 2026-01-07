# Strong4VM Documentation {#mainpage}

![Strong4VM Logo](../figures/icon.png)

*A tool for extracting Strong Transitive Dependency and Conflict Graphs from Variability Models*

## Welcome

Welcome to the Strong4VM documentation! Strong4VM is a comprehensive toolchain for analyzing feature model dependencies and conflicts through SAT-based backbone detection.

Variability models specify configurable options (features) along with their constraints. However, these constraints create numerous indirect relationships through chaining. **Strong4VM identifies all transitive strong relationships** between features and represents them as graphs. The term *transitive* refers to relationships formed by linking other relationships, while *strong* indicates validity in all configurations that comply with the variability model.

## Quick Links

- @ref getting_started "Getting Started" - Installation and first steps
- @ref architecture "Architecture Overview" - System design and components
- @ref examples "Usage Examples" - Common usage patterns
- @ref limitations "Limitations" - Known limitations and troubleshooting
- [API Reference](modules.html) - Complete API documentation

## Key Features

- **🎯 Unified Interface**: Single tool for UVL or DIMACS inputs
- **⚡ High Performance**: Multi-threaded graph generation with configurable parallelism
- **📈 Comprehensive**: Generates requires/excludes graphs and core/dead feature lists
- **✅ Well-tested**: Validated against [5,709 real-world variability models](https://doi.org/10.5281/zenodo.17277788)

## Components

Strong4VM consists of three integrated components forming a complete analysis pipeline:

1. **UVL2Dimacs** - Converts UVL feature models to DIMACS CNF format
   - ANTLR4-based parser for Universal Variability Language
   - Two transformation modes: Straightforward and Tseitin
   - Efficient CNF encoding of feature relationships

2. **Dimacs2Graphs** - Generates strong transitive graphs from CNF formulas
   - Multi-threaded parallel processing
   - SAT-based analysis for dependency and conflict detection
   - Produces Pajek .net graph files

3. **Backbone Solver** - High-performance backbone detection engine
   - Activity bumping heuristics for efficient solving
   - Identifies core (always enabled) and dead (never enabled) features
   - MiniSat-based implementation

See the @ref architecture "Architecture" page for detailed design information.

## Pipeline Overview

```
UVL Feature Model  →  [UVL2Dimacs]  →  DIMACS CNF  →  [Dimacs2Graphs + Backbone Solver]  →  Graphs
```

## Output Files

For input file `model.uvl` or `model.dimacs`, Strong4VM generates:

- `model__requires.net` - Strong transitive dependency graph (directed, Pajek format)
- `model__excludes.net` - Strong transitive conflict graph (undirected, Pajek format)
- `model__core.txt` - Core features (enabled in all configurations)
- `model__dead.txt` - Dead features (disabled in all configurations)

## Getting Help

- See @ref getting_started for installation and basic usage
- Check @ref examples for common usage patterns
- Review @ref limitations for troubleshooting
- Visit the [GitHub repository](https://github.com/rheradio/Strong4VM) for issues and updates

## License

Strong4VM is released under the MIT License. It is built upon:
- **MiniSat**: MIT License
- **ANTLR4**: BSD License

## Authors

- Ruben Heradio, rheradio@issi.uned.es
- Luis Cambelo, lcambelo1@alumno.uned.es
- Miguel A. Olivero, molivero@us.es
- José Manuel Sánchez Ruiz, jsanchez7@us.es
- Alberto Pérez García-Plaza, alberto.perez@lsi.uned.es
- David Fernández Amorós, david@issi.uned.es

## Funding

This work is funded by FEDER/Spanish Ministry of Science, Innovation and Universities (MCIN)/Agencia Estatal de Investigacion (AEI) under project COSY (PID2022-142043NB-I00).
