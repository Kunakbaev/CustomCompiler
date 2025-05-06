#ifndef SYNTAX_TREE_INCLUDE_SYNTAX_TREE_STRUCT_HPP
#define SYNTAX_TREE_INCLUDE_SYNTAX_TREE_STRUCT_HPP

#include "syntaxTreeNodeStruct.hpp"
#include "../../Dumper/include/dumperStruct.hpp"

struct SyntaxTree {
    size_t                       root;
    Node*                        memBuff;
    size_t                       memBuffSize;
    size_t                       freeNodeIndex;
    Dumper*                      dumper;
};


#endif
