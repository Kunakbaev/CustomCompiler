#ifndef SYNTAX_TREE_INCLUDE_SYNTAX_TREE_NODE_STRUCT_HPP
#define SYNTAX_TREE_INCLUDE_SYNTAX_TREE_NODE_STRUCT_HPP

#include <cstddef>
#include "../../LexemsRealizations/include/lexemsRealizations.hpp"

union NodeDataUnion {
    size_t     data;
    double     doubleData;
};

struct Node {
    Lexem        lexem;
    size_t       left;         // left son
    size_t       right;        // right son
    size_t       memBuffIndex; // ASK: is this field necessary
    size_t       parent;       // index in mem buff array of node's parent
};

#endif
