#include "../include/commonFileStart.hpp"

static SyntaxTreeErrors checkIfValidParent(const SyntaxTree* tree, size_t nodeInd) {
    IF_ARG_NULL_RETURN(tree);

    size_t parentInd = getSyntaxTreeNodePtr(tree, nodeInd)->parent;
    IF_NOT_COND_RETURN(parentInd <= tree->freeNodeIndex,
                       SYNTAX_TREE_BAD_PARENT_NODE);
    //LOG_DEBUG_VARS(parentInd, tree->root, nodeInd);
    if (tree->root == nodeInd) {
        IF_NOT_COND_RETURN(parentInd == 0, SYNTAX_TREE_BAD_PARENT_NODE);
        return SYNTAX_TREE_STATUS_OK;
    }

    Node parent = *getSyntaxTreeNodePtr(tree, parentInd);
    IF_NOT_COND_RETURN(parentInd != 0,
                       SYNTAX_TREE_BAD_PARENT_NODE);
    //LOG_DEBUG_VARS(parent.left, parent.right);
    IF_NOT_COND_RETURN((parent.left == nodeInd) ^ (parent.right == nodeInd),
                       SYNTAX_TREE_BAD_PARENT_NODE);

    return SYNTAX_TREE_STATUS_OK;
}

static SyntaxTreeErrors checkIfFuncNodeValid(const SyntaxTree* tree, const Node* node) {
    IF_ARG_NULL_RETURN(tree);
    IF_ARG_NULL_RETURN(node);

    // Function func = {};
    // ARIFM_OPS_ERR_CHECK(getFuncByIndex(node->data, &func));
    // LOG_DEBUG_VARS(func.type, func.name, node->data, node->doubleData);
    // if (func.type == BINARY_FUNC) {
    //     IF_NOT_COND_RETURN(node->left != 0 && node->right != 0,
    //                         ARIFM_TREE_FUNC_NODE_NOT_ENOUGH_ARGS);
    // } else {
    //     IF_NOT_COND_RETURN(node->left == 0 && node->right != 0,
    //                        ARIFM_TREE_UNARY_FUNC_NODE_BAD_ARGS);
    // }

    return SYNTAX_TREE_STATUS_OK;
}

// validates arifmetic tree, works not so fast
SyntaxTreeErrors validateSyntaxTree(const SyntaxTree* tree) {
    IF_ARG_NULL_RETURN(tree);

    // TODO:
//     if (tree->root == 0) // ASK: should this be error?
//         return SYNTAX_TREE_STATUS_OK;
//
//     for (size_t nodeInd = 1; nodeInd <= tree->freeNodeIndex; ++nodeInd) {
//         Node node = *getSyntaxTreeNodePtr(tree, nodeInd);
//         LOG_DEBUG_VARS(nodeInd);
//
//         IF_ERR_RETURN(checkIfValidParent(tree, nodeInd));
//
//         switch (node.nodeType) {
//             case ARIFM_TREE_NUMBER_NODE:
//                 IF_NOT_COND_RETURN(node.left == 0 && node.right == 0,
//                                    ARIFM_TREE_NUM_NODE_NOT_LEAF);
//                 break;
//             case ARIFM_TREE_VAR_NODE:
//                 IF_NOT_COND_RETURN(node.left == 0 && node.right == 0,
//                                    ARIFM_TREE_VAR_NODE_NOT_LEAF);
//                 break;
//             case ARIFM_TREE_FUNC_NODE:
//                 IF_ERR_RETURN(checkIfFuncNodeValid(tree, &node));
//                 break;
//             default:
//                 return ARIFM_TREE_INVALID_NODE_TYPE;
//         }
//     }

    return SYNTAX_TREE_STATUS_OK;
}
