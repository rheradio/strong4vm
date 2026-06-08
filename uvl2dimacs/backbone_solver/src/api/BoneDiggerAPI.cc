/**
 * @file BoneDiggerAPI.cc
 * @brief Implementation file for BoneDiggerAPI
 */

// File:   BoneDiggerAPI.cc
// Implementation of BoneDigger API

#include "BoneDiggerAPI.hh"
#include "DIMACSReader.hh"
#include "CheckCandidatesOneByOne.hh"
#include "FastOnCliffsSlowOnPlains.hh"
#include "RushAndPray.hh"
#include <iostream>
#include <iomanip>
#include <memory>
#include <set>

using std::cout;
using std::cerr;
using std::endl;

namespace bonedigger {

// Private implementation class (PIMPL pattern)
class BoneDiggerAPI::Impl {
public:
    Impl() : max_id(0), detector(nullptr), has_file(false), is_sat(false), attention_weight(1.0) {}

    ~Impl() {
        cleanup_detector();
        cleanup_reader();
    }

    bool read_file(const string& file_name) {
        cleanup_detector();
        cleanup_reader();

        try {
            // Open the file
            gzFile ff = gzopen(file_name.c_str(), "rb");
            if (ff == Z_NULL) {
                return false;
            }

            // Create reader and read the file
            reader = new Reader(ff);
            DIMACSReader dimacs_reader(*reader);
            dimacs_reader.read();

            // Store the data
            max_id = dimacs_reader.get_max_id();
            clauses = dimacs_reader.get_clause_vector();
            has_file = true;
            is_sat = false;

            return true;
        } catch (...) {
            cleanup_reader();
            return false;
        }
    }

    bool create_detector(const string& type) {
        if (!has_file) {
            return false;
        }

        cleanup_detector();

        if (type == "one" || type == "simple") {
            detector_type = ONE;
        } else if (type == "flatland") {
            detector_type = FLATLAND;
        } else if (type == "rush" || type == "rushandpray") {
            detector_type = RUSH;
        } else {
            return false;
        }

        return true;
    }

    vector<int> compute() {
        vector<int> result;

        if (!has_file || detector_type == NONE) {
            return result;
        }

        // Only cleanup if detector hasn't been created yet
        if (detector == nullptr) {
            try {
                if (detector_type == ONE) {
                    CheckCandidatesOneByOne* one_detector = new CheckCandidatesOneByOne(max_id, clauses, attention_weight);
                    detector = one_detector;

                    if (!one_detector->initialize()) {
                        is_sat = false;
                        cleanup_detector();
                        return result;
                    }

                    is_sat = true;
                    one_detector->run();

                    // Extract backbone
                    for (Var v = 1; v <= max_id; ++v) {
                        if (one_detector->is_backbone(v)) {
                            int lit = one_detector->backbone_sign(v) ? v : -v;
                            result.push_back(lit);
                        }
                    }

                } else if (detector_type == FLATLAND) {
                    FastOnCliffsSlowOnPlains* flatland_detector =
                        new FastOnCliffsSlowOnPlains(max_id, clauses, attention_weight);
                    detector = flatland_detector;

                    if (!flatland_detector->initialize()) {
                        is_sat = false;
                        cleanup_detector();
                        return result;
                    }

                    is_sat = true;
                    flatland_detector->run();

                    // Extract backbone
                    for (Var v = 1; v <= max_id; ++v) {
                        if (flatland_detector->is_backbone(v)) {
                            int lit = flatland_detector->backbone_sign(v) ? v : -v;
                            result.push_back(lit);
                        }
                    }

                } else if (detector_type == RUSH) {
                    RushAndPray* rush_detector =
                        new RushAndPray(max_id, clauses, attention_weight);
                    detector = rush_detector;

                    if (!rush_detector->initialize()) {
                        is_sat = false;
                        cleanup_detector();
                        return result;
                    }

                    is_sat = true;
                    rush_detector->run();

                    // Extract backbone
                    for (Var v = 1; v <= max_id; ++v) {
                        if (rush_detector->is_backbone(v)) {
                            int lit = rush_detector->backbone_sign(v) ? v : -v;
                            result.push_back(lit);
                        }
                    }

                }
            } catch (...) {
                cleanup_detector();
                return vector<int>();
            }
        } else {
            // Detector already exists, just extract backbone
            try {
                if (detector_type == ONE) {
                    CheckCandidatesOneByOne* one_detector = static_cast<CheckCandidatesOneByOne*>(detector);
                    for (Var v = 1; v <= max_id; ++v) {
                        if (one_detector->is_backbone(v)) {
                            int lit = one_detector->backbone_sign(v) ? v : -v;
                            result.push_back(lit);
                        }
                    }
                } else if (detector_type == FLATLAND) {
                    FastOnCliffsSlowOnPlains* flatland_detector =
                        static_cast<FastOnCliffsSlowOnPlains*>(detector);
                    for (Var v = 1; v <= max_id; ++v) {
                        if (flatland_detector->is_backbone(v)) {
                            int lit = flatland_detector->backbone_sign(v) ? v : -v;
                            result.push_back(lit);
                        }
                    }
                } else if (detector_type == RUSH) {
                    RushAndPray* rush_detector =
                        static_cast<RushAndPray*>(detector);
                    for (Var v = 1; v <= max_id; ++v) {
                        if (rush_detector->is_backbone(v)) {
                            int lit = rush_detector->backbone_sign(v) ? v : -v;
                            result.push_back(lit);
                        }
                    }
                }
            } catch (...) {
                return vector<int>();
            }
        }

        return result;
    }
    
    vector<int> compute_with_assumptions(const vector<int>& assumptions) {
        if (!has_file) return {};

        // Build a modified CNF by adding assumption unit clauses
        CNF modified_clauses = clauses;
        for (int assump : assumptions) {
            Var var = (Var)(std::abs(assump));  // MiniSat vars are 1-indexed here, matching DIMACS
            bool sign = (assump < 0);           // true = negative literal in MiniSat
            LiteralVector unit;
            unit.push_back(mkLit(var, sign));
            modified_clauses.push_back(LitSet(unit));
        }

        // Use the configured detector type; fall back to ONE if not yet set
        DetectorType fresh_type = (detector_type != NONE) ? detector_type : ONE;
        BackBone* det = nullptr;
        try {
            if      (fresh_type == ONE)      det = new CheckCandidatesOneByOne(max_id, modified_clauses, attention_weight);
            else if (fresh_type == FLATLAND) det = new FastOnCliffsSlowOnPlains(max_id, modified_clauses, attention_weight);
            else if (fresh_type == RUSH)     det = new RushAndPray(max_id, modified_clauses, attention_weight);
            else return {};
        } catch (...) {
            return {};
        }

        vector<int> result;
        try {
            if (det->initialize()) {
                det->run();
                for (Var v = 1; v <= max_id; ++v) {
                    if (det->is_backbone(v))
                        result.push_back(det->backbone_sign(v) ? (int)v : -(int)v);
                }
            }
        } catch (...) {}

        delete det;
        return result;
    }

    void set_weight(double w) { attention_weight = w; }
    int get_max_var() const { return max_id; }
    bool get_is_sat() const { return is_sat; }
    
    void print_bb() const {
        if (!is_sat) {
            cout << "Formula is UNSATISFIABLE" << endl;
            return;
        }
        
        if (!has_file || detector_type == NONE) {
            cout << "No backbone computed yet" << endl;
            return;
        }
        
        // Print formula statistics
        cout << "Formula statistics:" << endl;
        cout << "  Variables: " << max_id << endl;
        cout << "  Clauses: " << clauses.size() << endl;
        cout << "Formula is SATISFIABLE" << endl;
        
        // Count and collect backbone literals
        vector<int> backbone_lits;

        try {
            if (detector_type == ONE) {
                CheckCandidatesOneByOne* one_detector = static_cast<CheckCandidatesOneByOne*>(detector);
                if (one_detector) {
                    for (Var v = 1; v <= max_id; ++v) {
                        if (one_detector->is_backbone(v)) {
                            int lit = one_detector->backbone_sign(v) ? v : -v;
                            backbone_lits.push_back(lit);
                        }
                    }
                }
            } else if (detector_type == FLATLAND) {
                FastOnCliffsSlowOnPlains* flatland_detector =
                    static_cast<FastOnCliffsSlowOnPlains*>(detector);
                if (flatland_detector) {
                    for (Var v = 1; v <= max_id; ++v) {
                        if (flatland_detector->is_backbone(v)) {
                            int lit = flatland_detector->backbone_sign(v) ? v : -v;
                            backbone_lits.push_back(lit);
                        }
                    }
                }
            } else if (detector_type == RUSH) {
                RushAndPray* rush_detector =
                    static_cast<RushAndPray*>(detector);
                if (rush_detector) {
                    for (Var v = 1; v <= max_id; ++v) {
                        if (rush_detector->is_backbone(v)) {
                            int lit = rush_detector->backbone_sign(v) ? v : -v;
                            backbone_lits.push_back(lit);
                        }
                    }
                }
            }
        } catch (...) {
            cout << "Error retrieving backbone" << endl;
            return;
        }
        
        // Calculate percentage
        double percentage = (max_id > 0) ? (100.0 * backbone_lits.size() / max_id) : 0.0;
        
        cout << "Backbone size: " << backbone_lits.size() << " literals";
        cout << " (" << std::fixed << std::setprecision(2) << percentage << "% of variables)" << endl;
        
        if (backbone_lits.size() > 0) {
            cout << "Backbone literals: ";
            for (int lit : backbone_lits) {
                cout << lit << " ";
            }
            cout << endl;
        }
    }
    
private:
    enum DetectorType { NONE, ONE, FLATLAND, RUSH };

    void cleanup_detector() {
        if (detector) {
            if (detector_type == ONE) {
                delete static_cast<CheckCandidatesOneByOne*>(detector);
            } else if (detector_type == FLATLAND) {
                delete static_cast<FastOnCliffsSlowOnPlains*>(detector);
            } else if (detector_type == RUSH) {
                delete static_cast<RushAndPray*>(detector);
            }
            detector = nullptr;
        }
        detector_type = NONE;
    }
    
    void cleanup_reader() {
        if (reader) {
            delete reader;
            reader = nullptr;
        }
        clauses.clear();
        max_id = 0;
        has_file = false;
    }
    
    Reader* reader = nullptr;
    Var max_id;
    CNF clauses;
    void* detector;
    DetectorType detector_type = NONE;
    bool has_file;
    bool is_sat;
    double attention_weight;
};

// BoneDiggerAPI implementation

BoneDiggerAPI::BoneDiggerAPI() : pimpl(new Impl()) {}

BoneDiggerAPI::~BoneDiggerAPI() {
    delete pimpl;
}

bool BoneDiggerAPI::read_dimacs(const string& file_name) {
    return pimpl->read_file(file_name);
}

bool BoneDiggerAPI::create_backbone_detector(const string& bb_detector) {
    return pimpl->create_detector(bb_detector);
}

vector<int> BoneDiggerAPI::compute_backbone() {
    return pimpl->compute();
}

int BoneDiggerAPI::get_max_variable() const {
    return pimpl->get_max_var();
}

void BoneDiggerAPI::set_attention_weight(double weight) {
    pimpl->set_weight(weight);
}

bool BoneDiggerAPI::is_satisfiable() const {
    return pimpl->get_is_sat();
}

void BoneDiggerAPI::print_backbone() const {
    pimpl->print_bb();
}

vector<int> BoneDiggerAPI::compute_backbone_with_assumptions(const vector<int>& assumptions) {
    return pimpl->compute_with_assumptions(assumptions);
}

} // namespace bonedigger
