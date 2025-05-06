#include "../include/commonFileStart.hpp"
#include "../include/assignParents.hpp"

SyntaxTreeErrors assignParentNodes(SyntaxTree* tree, size_t nodeInd, size_t parentInd) {
    IF_ARG_NULL_RETURN(tree);

    if (!nodeInd)
        return SYNTAX_TREE_STATUS_OK;

    Node* node = getSyntaxTreeNodePtr(tree, nodeInd);
    //LOG_DEBUG_VARS(nodeInd, node->memBuffIndex, node->left, node->right);
    node->parent = parentInd;

    IF_ERR_RETURN(assignParentNodes(tree, node->left , nodeInd));
    IF_ERR_RETURN(assignParentNodes(tree, node->right, nodeInd));

    return SYNTAX_TREE_STATUS_OK;
}
