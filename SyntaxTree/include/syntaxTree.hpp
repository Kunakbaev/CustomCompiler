#ifndef SYNTAX_TREE_INCLUDE_SYNTAX_TREE_HPP
#define SYNTAX_TREE_INCLUDE_SYNTAX_TREE_HPP

#include "syntaxTreeStruct.hpp"
#include "syntaxTreeErrorsHandler.hpp"

SyntaxTreeErrors constructSyntaxTree(SyntaxTree* tree, Dumper* dumper);

SyntaxTreeErrors getNewNode(SyntaxTree* tree, size_t* newNodeIndex);

//SyntaxTreeErrors getPathToSyntaxTreeNode(const SyntaxTree* tree, const Node* node, size_t** path);
SyntaxTreeErrors addTreeLatexRepresentationToFile(const SyntaxTree* tree, const char* fileName);

// static SyntaxTreeErrors constructNodeWithKids(SyntaxTree* tree, size_t* newNodeInd, TreeNodeType nodeType, const void* data,
//                                              size_t leftSon, size_t rightSon);

SyntaxTreeErrors linkNewNodeToParent(
    SyntaxTree* tree,
    size_t      parentInd,
    bool        isLeftSon,
    size_t*     newNodeInd,
    const char* substr
);
SyntaxTreeErrors getCopyOfTree(const SyntaxTree* source, SyntaxTree* dest);
size_t getCopyOfSubtree(
    const SyntaxTree* tree,
    SyntaxTree*       destTree,
    size_t            srcNodeInd
);
Node* getSyntaxTreeNodePtr(const SyntaxTree* tree, size_t nodeInd);
size_t constructNodeWithKidsNoErrors(
    SyntaxTree*  tree,
    const Lexem* lexem,
    size_t       leftSon,
    size_t       rightSon
);
SyntaxTreeErrors saveSyntaxTree2File(const SyntaxTree* tree,
                                     const char* fileName);
SyntaxTreeErrors readSyntaxTreeFromFile(SyntaxTree* tree, const char* sourceFilePath);
SyntaxTreeErrors validateSyntaxTree(const SyntaxTree* tree);
SyntaxTreeErrors dumpSyntaxTreeInConsole(const SyntaxTree* tree);
SyntaxTreeErrors dumpSyntaxTree(SyntaxTree* tree);
SyntaxTreeErrors openImageOfCurrentStateSyntaxTree(SyntaxTree* tree);

SyntaxTreeErrors destructSyntaxTree(SyntaxTree* tree);

#endif
