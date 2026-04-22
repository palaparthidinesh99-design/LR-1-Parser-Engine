#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>

using namespace std;

struct Item {
    string lhs;
    vector<string> rhs;
    int dot;

    bool operator<(const Item& other) const {
        if (lhs != other.lhs) return lhs < other.lhs;
        if (dot != other.dot) return dot < other.dot;
        return rhs < other.rhs;
    }
    bool operator==(const Item& other) const {
        return lhs == other.lhs && rhs == other.rhs && dot == other.dot;
    }
};

struct Action {
    string type; // "shift", "reduce", "accept"
    int target;  // state for shift
    Item rule;   // rule for reduce
};

struct Precedence {
    int level;
    string associativity; // "left", "right"
};

struct TreeNode {
    string label;
    int id;
    vector<TreeNode*> children;
};

#endif // CORE_H
