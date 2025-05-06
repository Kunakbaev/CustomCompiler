#include <errno.h>

#include "../include/commonFileStart.hpp"

static const size_t MAX_LINE_LEN = 1 << 6;

static SyntaxTreeErrors readSyntaxTreeFromFileRecursive(
    SyntaxTree* tree,
    FILE* file,
    char* lineBuffer
) {
    IF_ARG_NULL_RETURN(tree);
    IF_ARG_NULL_RETURN(file);
    IF_ARG_NULL_RETURN(lineBuffer);

    size_t newNodeInd = 0;
    IF_ERR_RETURN(getNewNode(tree, &newNodeInd));
    Node* node = NULL;
    node = getSyntaxTreeNodePtr(tree, newNodeInd);

    LEXEMS_REALIZATIONS_ERR_CHECK(readLexemFromFile(file, &node->lexem, lineBuffer, MAX_LINE_LEN));

    fgets(lineBuffer, MAX_LINE_LEN, file); // reads empty line that delims two different nodes
    assert(lineBuffer != NULL);
    errno = 0;
    char* endPtr = NULL;
    int numOfChildren = strtol(lineBuffer, &endPtr, 10);

    fgets(lineBuffer, MAX_LINE_LEN, file); // reads empty line that delims two different nodes
    assert(lineBuffer != NULL);

    IF_NOT_COND_RETURN(numOfChildren == 0 || 
                       numOfChildren == 1 ||
                       numOfChildren == 2,
                       SYNTAX_TREE_INVALID_ARGUMENT); // TODO: add error

    if (numOfChildren >= 1) {
        IF_ERR_RETURN(readSyntaxTreeFromFileRecursive(tree, file, lineBuffer));
        node = getSyntaxTreeNodePtr(tree, newNodeInd);
        node->left = tree->root;
    }
    if (numOfChildren == 2) {
        IF_ERR_RETURN(readSyntaxTreeFromFileRecursive(tree, file, lineBuffer));
        node = getSyntaxTreeNodePtr(tree, newNodeInd);
        node->right = tree->root;
    }

    if (node->lexem.lexemSpecificName == DELIMS_OPEN_CURLY_BRACKET_LEXEM) { // because it works in prefix notation
        size_t tmp  = node->left; // change of children order (just swap)
        node->left  = node->right;
        node->right = tmp;
    }

    tree->root = node->memBuffIndex;

    return SYNTAX_TREE_STATUS_OK;
}

#include "../include/assignParents.hpp"

SyntaxTreeErrors readSyntaxTreeFromFile(SyntaxTree* tree, const char* sourceFilePath) {
    IF_ARG_NULL_RETURN(tree);
    IF_ARG_NULL_RETURN(sourceFilePath);

    FILE* file = fopen(sourceFilePath, "r");
    IF_NOT_COND_RETURN(file != NULL, SYNTAX_TREE_FILE_OPENING_ERROR);

    char* lineBuffer = (char*)calloc(MAX_LINE_LEN, sizeof(char));
    IF_NOT_COND_RETURN(lineBuffer != NULL, SYNTAX_TREE_MEMORY_ALLOCATION_ERROR);

    IF_ERR_RETURN(readSyntaxTreeFromFileRecursive(tree, file, lineBuffer));
    FREE(lineBuffer);

    fclose(file);

    IF_ERR_RETURN(assignParentNodes(tree, tree->root, 0));

    return SYNTAX_TREE_STATUS_OK;
}

