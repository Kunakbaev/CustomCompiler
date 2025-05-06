#include "../include/commonFileStart.hpp"

static SyntaxTreeErrors saveSyntaxTree2FileRecursive(
    const SyntaxTree* tree,
    FILE* file,
    size_t curNodeInd
) {
    IF_ARG_NULL_RETURN(tree);
    IF_ARG_NULL_RETURN(file);

    if (!curNodeInd)
        return SYNTAX_TREE_STATUS_OK;

    Node node = *getSyntaxTreeNodePtr(tree, curNodeInd);
    saveLexemToFile(file, &node.lexem);

    int numOfNodes = (node.left != 0) + (node.right != 0);
    fprintf(file, "%d\n\n", numOfNodes);

    IF_ERR_RETURN(saveSyntaxTree2FileRecursive(tree, file, node.left));
    IF_ERR_RETURN(saveSyntaxTree2FileRecursive(tree, file, node.right));

    return SYNTAX_TREE_STATUS_OK;
}

SyntaxTreeErrors saveSyntaxTree2File(const SyntaxTree* tree,
                                     const char* fileName) {
    IF_ARG_NULL_RETURN(tree);

    FILE* file = fopen(fileName, "w");
    IF_NOT_COND_RETURN(file != NULL, SYNTAX_TREE_FILE_OPENING_ERROR);

    IF_ERR_RETURN(saveSyntaxTree2FileRecursive(tree, file, tree->root));
    fclose(file);

    return SYNTAX_TREE_STATUS_OK;
}
