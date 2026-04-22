#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <set>
#include <map>
#include <string>
#include "core.h"
#include "grammar.h"

using namespace std;

class Parser {
public:
    GrammarManager* gm;
    vector<set<Item>> C;
    map<pair<int, string>, int> goto_table;
    map<pair<int, string>, vector<Action>> action_table;

    Parser(GrammarManager* manager) : gm(manager) {}

    set<Item> closure(set<Item> I);
    set<Item> goto_operation(set<Item> I, string X);
    
    void build_automaton();
    void build_tables();
    void print_automaton_and_tables();
    void parse_input(vector<string> input);
};

#endif // PARSER_H
