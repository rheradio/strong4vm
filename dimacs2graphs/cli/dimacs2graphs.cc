/*
 *  dimacs2graphs.cc
 *  Command-line interface for generating transitive graphs
 *
 *  Input: Satisfiable formula (Read from file, first arg)
 *  Output: Files for graph representations (igraph, gephi, pajek,...)
 *
 */

#include "../api/Dimacs2GraphsAPI.hh"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;
using namespace dimacs2graphs;

int main(int argc, char **argv) {
	if (argc < 2 || argc > 3) {
		cout << "Generate Transitive Graphs v3 (using BackboneSolver)." << endl;
		cout << "This program produces source files for Transitive Graphs from a CNF SAT formula." << endl;
		cout << endl;
		cout << "USAGE: ./dimacs2graphs <dimacs_file> [num_threads]" << endl;
		cout << "  dimacs_file  - Path without .dimacs extension" << endl;
		cout << "  num_threads  - Number of threads (default: 1)" << endl;
		return 1;
	}

	string file_name(argv[1]);
	int num_threads = 1;

	if (argc == 3) {
		num_threads = atoi(argv[2]);
		if (num_threads < 1) {
			cerr << "Error: num_threads must be at least 1" << endl;
			return 1;
		}
	}

	// Create API instance
	Dimacs2GraphsAPI api;

	// Generate graphs using default detector ("one") and output to same directory as input
	if (!api.generate_graphs(file_name, "", "one", num_threads)) {
		cerr << "Error: " << api.get_error_message() << endl;
		return 2;
	}

	return 0;
}
