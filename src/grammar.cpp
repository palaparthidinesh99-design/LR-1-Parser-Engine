#include "../include/grammar.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

bool GrammarManager::load_from_file(const string& filepath) {
    ifstream infile(filepath);
    if (!infile.is_open()) {
        cerr << "Error: Cannot open " << filepath << endl;
        return false;
    }

    string line;
    int section = 0; // 1: Terminals, 2: NonTerminals, 3: Start, 4: Precedence, 5: Rules

    while (getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        if (line == "TERMINALS") { section = 1; continue; }
        if (line == "NON_TERMINALS") { section = 2; continue; }
        if (line == "START") { section = 3; continue; }
        if (line == "PRECEDENCE") { section = 4; continue; }
        if (line == "RULES") { section = 5; continue; }
        if (line == "END") { section = 0; continue; }

        istringstream iss(line);
        if (section == 1) {
            string t;
            while (iss >> t) terminals.insert(t);
        } else if (section == 2) {
            string nt;
            while (iss >> nt) non_terminals.insert(nt);
        } else if (section == 3) {
            iss >> start_symbol;
        } else if (section == 4) {
            string op, assoc;
            int level;
            if (iss >> op >> level >> assoc) {
                precedence_rules[op] = {level, assoc};
            }
        } else if (section == 5) {
            string lhs, arrow, rhs_sym;
            iss >> lhs >> arrow;
            vector<string> rhs;
            while (iss >> rhs_sym) {
                rhs.push_back(rhs_sym);
            }
            if (arrow == "->") {
                grammar[lhs].push_back(rhs);
                numbered_rules.push_back({lhs, rhs});
            }
        }
    }
    return true;
}

void GrammarManager::compute_first() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto const& pair : grammar) {
            const string& nt = pair.first;
            const vector<vector<string>>& prods = pair.second;
            for (auto const& prod : prods) {
                if (prod.empty()) continue;
                string first_sym = prod[0];
                size_t old_size = first_set[nt].size();
                
                if (terminals.count(first_sym)) {
                    first_set[nt].insert(first_sym);
                } else if (non_terminals.count(first_sym)) {
                    for (const string& f : first_set[first_sym]) {
                        first_set[nt].insert(f);
                    }
                }
                if (first_set[nt].size() > old_size) changed = true;
            }
        }
    }
}

void GrammarManager::compute_follow() {
    follow_set[start_symbol].insert("$");
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto const& pair : grammar) {
            const string& A = pair.first;
            const vector<vector<string>>& prods = pair.second;
            for (auto const& prod : prods) {
                for (size_t i = 0; i < prod.size(); ++i) {
                    string B = prod[i];
                    if (non_terminals.count(B)) {
                        size_t old_size = follow_set[B].size();
                        if (i + 1 < prod.size()) {
                            string beta = prod[i+1];
                            if (terminals.count(beta)) {
                                follow_set[B].insert(beta);
                            } else if (non_terminals.count(beta)) {
                                for (const string& f : first_set[beta]) {
                                    follow_set[B].insert(f);
                                }
                            }
                        } else {
                            for (const string& f : follow_set[A]) {
                                follow_set[B].insert(f);
                            }
                        }
                        if (follow_set[B].size() > old_size) changed = true;
                    }
                }
            }
        }
    }
}

void GrammarManager::augment_grammar() {
    grammar["S'"].push_back({start_symbol});
    non_terminals.insert("S'");
    numbered_rules.insert(numbered_rules.begin(), {"S'", {start_symbol}});
}
