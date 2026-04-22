#include "../include/parser.h"
#include <iostream>
#include <iomanip>
#include <queue>

using namespace std;

int get_rule_id(const GrammarManager* gm, const Item& rule) {
    for (size_t i = 0; i < gm->numbered_rules.size(); ++i) {
        if (gm->numbered_rules[i].first == rule.lhs && gm->numbered_rules[i].second == rule.rhs) {
            return i;
        }
    }
    return -1;
}

set<Item> Parser::closure(set<Item> I) {
    set<Item> C = I;
    bool changed = true;
    while (changed) {
        changed = false;
        set<Item> temp_C = C;
        for (const auto& item : C) {
            if (item.dot < item.rhs.size()) {
                string B = item.rhs[item.dot];
                if (gm->non_terminals.count(B)) {
                    for (const auto& prod : gm->grammar[B]) {
                        Item new_item = {B, prod, 0};
                        if (temp_C.insert(new_item).second) {
                            changed = true;
                        }
                    }
                }
            }
        }
        C = temp_C;
    }
    return C;
}

set<Item> Parser::goto_operation(set<Item> I, string X) {
    set<Item> moved;
    for (const auto& item : I) {
        if (item.dot < item.rhs.size() && item.rhs[item.dot] == X) {
            moved.insert({item.lhs, item.rhs, item.dot + 1});
        }
    }
    return closure(moved);
}

void Parser::build_automaton() {
    Item start_item = {"S'", {gm->start_symbol}, 0};
    set<Item> I0 = closure({start_item});
    C.push_back(I0);
    
    queue<int> q;
    q.push(0);
    
    vector<string> symbols;
    for (const string& t : gm->terminals) symbols.push_back(t);
    for (const string& nt : gm->non_terminals) symbols.push_back(nt);
    
    while (!q.empty()) {
        int i = q.front();
        q.pop();
        
        for (const string& X : symbols) {
            set<Item> next_I = goto_operation(C[i], X);
            if (!next_I.empty()) {
                int target_idx = -1;
                for (size_t j = 0; j < C.size(); ++j) {
                    if (C[j] == next_I) { target_idx = j; break; }
                }
                if (target_idx == -1) {
                    C.push_back(next_I);
                    target_idx = C.size() - 1;
                    q.push(target_idx);
                }
                if (gm->terminals.count(X) || X == "$") {
                    goto_table[{i, X}] = target_idx;
                } else if (gm->non_terminals.count(X)) {
                    goto_table[{i, X}] = target_idx;
                }
            }
        }
    }
}

void Parser::build_tables() {
    for (size_t i = 0; i < C.size(); ++i) {
        for (const auto& item : C[i]) {
            if (item.dot < item.rhs.size()) {
                string a = item.rhs[item.dot];
                if (gm->terminals.count(a)) {
                    if (goto_table.count({i, a})) {
                        int j = goto_table[{i, a}];
                        action_table[{i, a}].push_back({"shift", j, {}});
                    }
                }
            } else {
                if (item.lhs == "S'" && item.rhs == vector<string>{gm->start_symbol}) {
                    action_table[{i, "$"}].push_back({"accept", 0, {}});
                } else {
                    for (const string& b : gm->follow_set[item.lhs]) {
                        action_table[{i, b}].push_back({"reduce", 0, item});
                    }
                }
            }
        }
    }
    
    // Conflict resolution
    for (auto& pair : action_table) {
        int state = pair.first.first;
        string symbol = pair.first.second;
        vector<Action>& acts = pair.second;
        
        if (acts.size() > 1) {
            cout << "Conflict at state " << state << " on symbol " << symbol << endl;
            // Shift-reduce detection
            Action* shift_act = nullptr;
            Action* reduce_act = nullptr;
            for (auto& a : acts) {
                if (a.type == "shift") shift_act = &a;
                if (a.type == "reduce") reduce_act = &a;
            }
            if (shift_act && reduce_act) {
                cout << "Resolving Shift/Reduce conflict using precedence!" << endl;
                string op_shift = symbol;
                string op_reduce = "";
                for (const auto& r : reduce_act->rule.rhs) {
                    if (gm->precedence_rules.count(r)) op_reduce = r;
                }
                
                int p_shift = gm->precedence_rules.count(op_shift) ? gm->precedence_rules[op_shift].level : 0;
                int p_reduce = gm->precedence_rules.count(op_reduce) ? gm->precedence_rules[op_reduce].level : 0;
                
                Action chosen_act;
                if (p_shift > p_reduce) {
                    chosen_act = *shift_act;
                } else if (p_reduce > p_shift) {
                    chosen_act = *reduce_act;
                } else {
                    string assoc = gm->precedence_rules.count(op_shift) ? gm->precedence_rules[op_shift].associativity : "none";
                    if (assoc == "left") {
                        chosen_act = *reduce_act;
                    } else {
                        chosen_act = *shift_act;
                    }
                }
                acts = {chosen_act};
            } else {
                cout << "Unresolved Reduce/Reduce Conflict!" << endl;
                acts = {acts[0]};
            }
        }
    }
}

void Parser::print_automaton_and_tables() {
    cout << "\n--- NUMBERED PRODUCTION RULES ---" << endl;
    for (size_t i = 0; i < gm->numbered_rules.size(); ++i) {
        cout << "(" << i << ") " << gm->numbered_rules[i].first << " -> ";
        for (const string& s : gm->numbered_rules[i].second) cout << s << " ";
        cout << endl;
    }

    cout << "\n--- DFA ITEMS AND TRANSITIONS ---" << endl;
    for (size_t i = 0; i < C.size(); ++i) {
        cout << "I" << i << ":" << endl;
        for (const auto& item : C[i]) {
            cout << "  " << item.lhs << " -> ";
            for (size_t k = 0; k < item.rhs.size(); ++k) {
                if (k == item.dot) cout << ". ";
                cout << item.rhs[k] << " ";
            }
            if (item.dot == item.rhs.size()) cout << ".";
            cout << endl;
        }
        
        bool has_trans = false;
        for (const string& t : gm->terminals) {
            if (goto_table.count({i, t})) {
                cout << "  goto(" << t << ") -> I" << goto_table[{i, t}] << endl;
                has_trans = true;
            }
        }
        for (const string& nt : gm->non_terminals) {
            if (goto_table.count({i, nt})) {
                cout << "  goto(" << nt << ") -> I" << goto_table[{i, nt}] << endl;
                has_trans = true;
            }
        }
        if (!has_trans) cout << "  (No transitions)" << endl;
        cout << endl;
    }

    cout << "--- ACTION AND GOTO TABLES ---" << endl;
    vector<string> header_terms;
    for (const string& t : gm->terminals) {
        if (t != "$") header_terms.push_back(t);
    }
    header_terms.push_back("$");
    
    vector<string> header_non_terms;
    for (const string& nt : gm->non_terminals) {
        if (nt != "S'") header_non_terms.push_back(nt);
    }
    
    cout << left << setw(6) << "State";
    for (const string& t : header_terms) cout << "\t" << setw(8) << t;
    cout << "|||";
    for (const string& nt : header_non_terms) cout << "\t" << setw(8) << nt;
    cout << endl;
    
    for (size_t i = 0; i < C.size(); ++i) {
        cout << left << setw(6) << i;
        for (const string& t : header_terms) {
            cout << "\t";
            if (action_table.count({i, t}) && !action_table[{i, t}].empty()) {
                Action act = action_table[{i, t}][0];
                if (act.type == "shift") {
                    cout << left << setw(8) << ("S" + to_string(act.target));
                } else if (act.type == "reduce") {
                    int rid = get_rule_id(gm, act.rule);
                    cout << left << setw(8) << ("R" + to_string(rid));
                } else if (act.type == "accept") {
                    cout << left << setw(8) << "ACC";
                }
            } else {
                cout << left << setw(8) << "";
            }
        }
        cout << "|||";
        for (const string& nt : header_non_terms) {
            cout << "\t";
            if (goto_table.count({i, nt})) {
                cout << left << setw(8) << goto_table[{i, nt}];
            } else {
                cout << left << setw(8) << "";
            }
        }
        cout << endl;
    }
}

void dfs_mermaid(TreeNode* curr, vector<TreeNode*>& nodes) {
    if(!curr) return;
    nodes.push_back(curr);
    for(TreeNode* c : curr->children) {
        dfs_mermaid(c, nodes);
    }
}

void print_mermaid_tree(TreeNode* root) {
    cout << "\n--- DERIVATION TREE ---" << endl;
    cout << "graph TD" << endl;
    // Premium spaced CSS
    cout << "  classDef terminal fill:#0f172a,stroke:#3b82f6,stroke-width:2px,color:#93c5fd,rx:8px,ry:8px,font-size:16px,padding:12px;" << endl;
    cout << "  classDef nonterm fill:#1e293b,stroke:#a855f7,stroke-width:2px,color:#e879f9,rx:8px,ry:8px,font-size:16px,padding:12px;" << endl;
    
    vector<TreeNode*> nodes;
    dfs_mermaid(root, nodes);
    
    // We don't need 'leaves' constraint, just define styles
    for(TreeNode* n : nodes) {
        if (n->children.empty()) {
            cout << "  " << n->id << "[\"" << n->label << "\"]:::terminal" << endl;
        } else {
            cout << "  " << n->id << "[\"" << n->label << "\"]:::nonterm" << endl;
        }
    }
    for(TreeNode* n : nodes) {
        for(TreeNode* c : n->children) {
            cout << "  " << n->id << " --> " << c->id << endl;
        }
    }
}

void Parser::parse_input(vector<string> input) {
    if (input.empty() || input.back() != "$") {
        input.push_back("$");
    }
    
    vector<int> stack_states = {0};
    vector<string> stack_symbols;
    
    vector<TreeNode*> tree_stack;
    int next_node_id = 0;
    
    cout << "\n--- PARSER SIMULATION ---" << endl;
    cout << "Stack\tInput\tAction" << endl;
    
    int cursor = 0;
    while (true) {
        int state = stack_states.back();
        string sym = input[cursor];
        
        string stack_str = to_string(stack_states[0]);
        for (size_t i = 1; i < stack_states.size(); ++i) {
            stack_str += " " + stack_symbols[i-1] + " " + to_string(stack_states[i]);
        }
        
        string input_str = "";
        for (int i = cursor; i < input.size(); ++i) {
            input_str += input[i] + (i == input.size() -1 ? "" : " ");
        }
        
        cout << stack_str << "\t" << input_str << "\t";
        
        if (!action_table.count({state, sym}) || action_table[{state, sym}].empty()) {
            cout << "Error: No action defined." << endl;
            break;
        }
        
        Action act = action_table[{state, sym}][0];
        
        if (act.type == "shift") {
            cout << "shift " << act.target << endl;
            stack_states.push_back(act.target);
            stack_symbols.push_back(sym);
            
            tree_stack.push_back(new TreeNode{sym, next_node_id++, {}});
            
            cursor++;
        } else if (act.type == "reduce") {
            int rid = get_rule_id(gm, act.rule);
            cout << "reduce " << act.rule.lhs << " -> ";
            for (const string& r : act.rule.rhs) cout << r << " ";
            cout << " (Rule " << rid << ")" << endl;
            
            TreeNode* parent = new TreeNode{act.rule.lhs, next_node_id++, {}};
            
            for (size_t i = 0; i < act.rule.rhs.size(); ++i) {
                stack_states.pop_back();
                stack_symbols.pop_back();
                parent->children.insert(parent->children.begin(), tree_stack.back());
                tree_stack.pop_back();
            }
            int t = stack_states.back();
            string A = act.rule.lhs;
            stack_symbols.push_back(A);
            tree_stack.push_back(parent);
            
            if (!goto_table.count({t, A})) {
                cout << "Error: No goto defined." << endl;
                break;
            }
            stack_states.push_back(goto_table[{t, A}]);
        } else if (act.type == "accept") {
            cout << "accept" << endl;
            if(!tree_stack.empty()) {
                print_mermaid_tree(tree_stack.back());
            }
            break;
        }
    }
}
