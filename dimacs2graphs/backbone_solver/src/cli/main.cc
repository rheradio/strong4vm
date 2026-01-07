/**
 * @file main.cc
 * @brief Implementation file for main
 */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include <chrono>
#include <iomanip>
#include <iostream>

#include "BackBone.hh"
#include "CheckCandidatesOneByOne.hh"
#include "CheckCandidatesOneByOneWithoutAttention.hh"
#include "DIMACSReader.hh"

using std::cerr;
using std::cout;
using std::endl;
using std::setprecision;
using namespace backbone_solver;

// ANSI color codes
static const char* COLOR_BLUE = "\033[34m";
static const char* COLOR_YELLOW = "\033[33m";
static const char* COLOR_RED = "\033[31m";
static const char* COLOR_RESET = "\033[0m";

static const char* output_prefix = "c ";

enum DetectorType { ONE_BY_ONE, WITHOUT_ATTENTION };

static string input_file_name;
static bool print_help = false;
static DetectorType detector_type = ONE_BY_ONE;  // Default to CheckCandidatesOneByOne
static BackBone* pdetector = NULL;
static Range range;
static bool instance_sat = false;

void print_usage();
void print_header();
bool parse_options(int argc, char** argv);
void print_backbone(const BackBone& worker, const Range& range,
                    ostream& output);
int run_detector(ostream& output);
void register_sig_handlers();
BackBone* create_detector(DetectorType type, Var max_id, const CNF& clauses);

int main(int argc, char** argv) {
  print_header();
  register_sig_handlers();

  if (!parse_options(argc, argv) || print_help) {
    print_usage();
    return 100;
  }

  cout << COLOR_BLUE << output_prefix << endl << COLOR_RESET;
  cout << COLOR_BLUE << output_prefix << "Formula: " << input_file_name << endl << COLOR_RESET;
  cout << COLOR_BLUE << output_prefix << endl << COLOR_RESET;
  ostream& output = std::cout;
  int return_value = run_detector(output);
  return return_value;
}

Reader* make_reader(string flafile) {
  gzFile ff = Z_NULL;
  if (flafile.size() == 1 && flafile[0] == '-') {
    return new Reader(std::cin);
  } else {
    ff = gzopen(flafile.c_str(), "rb");
    if (ff == Z_NULL) {
      cerr << "Unable to open file: " << flafile << endl;
      exit(100);
    }
    return new Reader(ff);
  }
  assert(0);
  return NULL;
}

BackBone* create_detector(DetectorType type, Var max_id, const CNF& clauses) {
  switch (type) {
    case ONE_BY_ONE:
      return new CheckCandidatesOneByOne(max_id, clauses);
    case WITHOUT_ATTENTION:
      return new CheckCandidatesOneByOneWithoutAttention(max_id, clauses);
    default:
      return new CheckCandidatesOneByOne(max_id, clauses);
  }
}

int run_detector(ostream& output) {
  using std::chrono::duration;
  using std::chrono::high_resolution_clock;

  auto start_total = high_resolution_clock::now();

  // read input
  auto start_read = high_resolution_clock::now();
  Reader* fr = make_reader(input_file_name);
  DIMACSReader reader(*fr);
  reader.read();
  auto end_read = high_resolution_clock::now();
  duration<double> time_read = end_read - start_read;

  // determine which part of variables to compute backbone of
  range = Range(1, reader.get_max_id());
  output << COLOR_BLUE << "c Range: " << range.first << "-" << range.second << endl << COLOR_RESET;

  // Create the detector using factory function
  pdetector = create_detector(detector_type, reader.get_max_id(),
                              reader.get_clause_vector());
  BackBone& detector = *pdetector;

  // Initialize detector
  auto start_init = high_resolution_clock::now();
  bool is_sat = detector.initialize();
  auto end_init = high_resolution_clock::now();
  duration<double> time_init = end_init - start_init;

  if (!is_sat) {  // unsatisfiable
    output << COLOR_BLUE << output_prefix << "ERROR: the formula is unsatisfiable" << endl << COLOR_RESET;
    delete pdetector;
    delete fr;

    auto end_total = high_resolution_clock::now();
    duration<double> time_total = end_total - start_total;
    output << COLOR_BLUE << output_prefix << endl << COLOR_RESET;
    output << COLOR_YELLOW << "t Time: " << endl << COLOR_RESET;
    output << COLOR_YELLOW << "t    Read DIMACs file: " << std::fixed << setprecision(6)
           << time_read.count() << " seconds" << endl << COLOR_RESET;
    output << COLOR_YELLOW << "t    Initialize and call MiniSat: " << std::fixed
           << setprecision(6) << time_init.count() << " seconds" << endl << COLOR_RESET;
    output << COLOR_YELLOW << "t    Total: " << std::fixed << setprecision(6)
           << time_total.count() << " seconds" << endl << COLOR_RESET;
    return 20;
  }
  instance_sat = true;

  // run detector
  auto start_run = high_resolution_clock::now();
  detector.run();
  auto end_run = high_resolution_clock::now();
  duration<double> time_run = end_run - start_run;

  output << COLOR_BLUE << output_prefix << "Computation completed " << endl << COLOR_RESET;

  // print results
  print_backbone(detector, range, output);

  delete pdetector;
  delete fr;

  auto end_total = high_resolution_clock::now();
  duration<double> time_total = end_total - start_total;
  output << COLOR_BLUE << output_prefix << endl << COLOR_RESET;
  output << COLOR_YELLOW << "t Time: " << endl << COLOR_RESET;
  output << COLOR_YELLOW << "t    Read DIMACs file: " << std::fixed << setprecision(6)
         << time_read.count() << " seconds" << endl << COLOR_RESET;
  output << COLOR_YELLOW << "t    Initialize MiniSat and compute the backbone: " << std::fixed
         << setprecision(6) << time_init.count() + time_run.count()
         << " seconds" << endl << COLOR_RESET;
  output << COLOR_YELLOW << "t    Total: " << std::fixed << setprecision(6)
         << time_total.count() << " seconds" << endl << COLOR_RESET;
  return 10;
}

void print_backbone(const BackBone& worker, const Range& range,
                    ostream& output) {
  output << COLOR_BLUE << output_prefix << endl << COLOR_RESET;
  output << COLOR_RED << "v ";
  size_t counter = 0;
  for (Var v = range.first; v <= range.second; ++v) {
    if (worker.is_backbone(v)) {
      ++counter;
      output << ' ';
      output << (worker.backbone_sign(v) ? "" : "-") << v;
    }
  }
  output << endl << COLOR_RESET;
  const float percentage =
      100.0 * (float)counter / (float)(range.second - range.first + 1);
  output << COLOR_BLUE << output_prefix << endl << COLOR_RESET;
  output << COLOR_BLUE << output_prefix << "Backbone size: " << counter << ", "
         << setprecision(2) << percentage << "% of the range" << endl << COLOR_RESET;
}

// Purpose: Print runtime header when executing BackboneSolver.

bool parse_options(int argc, char** argv) {
  opterr = 0;
  int c;
  while ((c = getopt(argc, argv, "how")) != -1) {
    switch (c) {
      case 'h':
        print_help = true;
        break;
      case 'o':
        detector_type = ONE_BY_ONE;
        break;
      case 'w':
        detector_type = WITHOUT_ATTENTION;
        break;
      case '?':
        if (isprint(optopt))
          fprintf(stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
        return false;
      default:
        return false;
    }
  }

  if (!print_help && (optind >= argc)) {
    cerr << "ERROR: file name expected" << endl;
    return false;
  } else {
    if (!print_help) input_file_name = argv[optind];
  }
  return true;
}

// Purpose: Handler for external signals, namely SIGHUP and SIGINT.

static void SIG_handler(int signum);
static void finishup();

void register_sig_handlers() {
  signal(SIGHUP, SIG_handler);
  signal(SIGINT, SIG_handler);
  signal(SIGQUIT, SIG_handler);
  signal(SIGTERM, SIG_handler);
  signal(SIGABRT, SIG_handler);
  signal(SIGALRM, SIG_handler);
}

// Generic handler, for all process termination signals that can be caught
static void SIG_handler(int signum) {
  string signame;
  switch (signum) {
    case 1:
      signame = "SIGHUP";
      break;
    case 2:
      signame = "SIGINT";
      break;
    case 3:
      signame = "SIGQUIT";
      break;
    case 9:
      signame = "SIGKILL";
      break;
    case 15:
      signame = "SIGTERM";
      break;
    default:
      cout << "Invalid signal received...\n";
      exit(2);
      break;
  }
  cout << "Bonedigger received external signal " << signame << ". " << endl;
  cout << "Terminating and printing whatever computed so far." << endl;
  finishup();
}

static void finishup() {
  if (instance_sat && pdetector) {
    print_backbone(*pdetector, range, cout);
  } else {
    cout << "s 0" << endl;
  }
  exit(instance_sat ? 10 : 20);
}

void print_header() {
  cerr << "c     ____                          " << std::endl;
  cerr << "c    | __ )  ___  _ __   ___        " << std::endl;
  cerr << "c    |  _ \\ / _ \\| '_ \\ / _ \\       " << std::endl;
  cerr << "c    | |_) | (_) | | | |  __/       " << std::endl;
  cerr << "c    |____/ \\___/|_| |_|\\___|       " << std::endl;
  cerr << "c    |  _ \\(_) __ _  __ _  ___ _ __ " << std::endl;
  cerr << "c    | | | | |/ _` |/ _` |/ _ \\ '__|  #         " << std::endl;
  cerr << "c    | |_| | | (_| | (_| |  __/ |    #####       " << std::endl;
  cerr << "c    |____/|_|\\__, |\\__, |\\___|_|  #####      " << std::endl;
  cerr << "c             |___/ |___/          ##########    " << std::endl;
  cerr << "c                                   ###########   " << std::endl;
  cerr << "c              ##########       #### #############" << std::endl;
  cerr << "c             ###      ###    #######  ########## " << std::endl;
  cerr << "c       ##### ###      ###   ########     ####    " << std::endl;
  cerr << "c         #######       ###  #######              " << std::endl;
  cerr << "c         #######        ### #######              " << std::endl;
  cerr << "c       #########        #######                  " << std::endl;
  cerr << "c     ###########        #############            " << std::endl;
  cerr << "c     #################################           " << std::endl;
  cerr << "c     ##################################          " << std::endl;
  cerr << "c     ##### ##### ########## ##### ######         " << std::endl;
  cerr << "c    #### ######### ###### ######### ####         " << std::endl;
  cerr << "c    ### ########### ################ ###         " << std::endl;
  cerr << "c     ## ########### #### ########### ##          " << std::endl;
  cerr << "c         #########        #########              " << std::endl;
  cerr << "c          #######          #######               " << std::endl;
  cerr << "c                                             " << std::endl;
  cerr << "c A TOOL FOR COMPUTING THE BACKBONE OF A BOOLEAN FORMULA, 2026"
       << std::endl;
  cerr << "c Ruben Heradio, rheradio@issi.uned.es" << std::endl;
  cerr << "c Luis Cambelo, lcambelo1@alumno.uned.es" << std::endl;
  cerr << "c Dictino Chaos, dchaos@dia.uned.es" << std::endl;
}

void print_usage() {
  cout << "USAGE" << "\tbonedigger <file name>" << endl;
  cout << "    -o ... use << check candidates One by one >> (default)" << endl;
  cout << "    -w ... use << check candidates one by one Without attention >>"
       << endl;
  cout << "    -h ... show this help message" << endl;
  cout << "NOTES:" << endl;
  cout << "   if filename is '-', instance is read from the standard input "
          "(press Ctrl+D to finish)"
       << endl;
}