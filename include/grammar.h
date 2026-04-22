#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include "core.h"

using namespace std;

class GrammarManager {
public:
    map<string, vector<vector<string>>> grammar;
    vector<pair<string, vector<string>>> numbered_rules; // For rule numbering output
    set<string> non_terminals;
    set<string> terminals;
    string start_symbol;

    map<string, set<string>> first_set;
    map<string, set<string>> follow_set;
    
    map<string, Precedence> precedence_rules;

    bool load_from_file(const string& filepath);
    void compute_first();
    void compute_follow();
    
    void augment_grammar();
};

#endif // GRAMMAR_H
