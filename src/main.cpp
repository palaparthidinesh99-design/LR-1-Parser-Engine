#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/grammar.h"
#include "../include/parser.h"

using namespace std;

int main() {
    GrammarManager gm;
    
    // Load grammar from file
    if (!gm.load_from_file("data/grammar.txt")) {
        cout << "Failed to load grammar.txt" << endl;
        return 1;
    }
    
    gm.augment_grammar();
    gm.compute_first();
    gm.compute_follow();

    cout << "FIRST:" << endl;
    for (const auto& nt : gm.non_terminals) {
        if(nt == "S'") continue;
        cout << nt << ": ";
        for (const string& f : gm.first_set[nt]) cout << f << " ";
        cout << endl;
    }
    
    cout << "\nFOLLOW:" << endl;
    for (const auto& nt : gm.non_terminals) {
        if(nt == "S'") continue;
        cout << nt << ": ";
        for (const string& f : gm.follow_set[nt]) cout << f << " ";
        cout << endl;
    }

    Parser parser(&gm);
    parser.build_automaton();
    parser.build_tables();

    parser.print_automaton_and_tables();

    // Read input string from data/input.txt
    vector<string> input_tokens;
    ifstream infile("data/input.txt");
    if (infile.is_open()) {
        string t;
        while (infile >> t) {
            input_tokens.push_back(t);
        }
    }
    
    // Default fallback if empty
    if (input_tokens.empty()) {
        input_tokens = {"id", "+", "id", "*", "id", "$"};
    }
    
    parser.parse_input(input_tokens);

    return 0;
}
