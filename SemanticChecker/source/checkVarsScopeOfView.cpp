#include "../include/commonFileStart.hpp"
//#include "../include/semanticChecker.hpp"
//#include "semanticChecker.cpp"

static SemanticCheckerErrors isDeclarationLexem(
    SemanticChecker* checker,
    const Lexem*     lexem,
    bool*            isDeclLex
) {
    IF_ARG_NULL_RETURN(checker);
    IF_ARG_NULL_RETURN(lexem);
    IF_ARG_NULL_RETURN(isDeclLex);

    Identificator res = {};
    SemanticCheckerErrors err = getIdentificatorByLexem(checker, lexem, &res);
    *isDeclLex = err == SEMANTIC_CHECKER_STATUS_OK;

    return SEMANTIC_CHECKER_STATUS_OK;
}

static SemanticCheckerErrors isAncestorInSyntaxTree(
    const SyntaxTree* tree,
    const Node*       father,
    const Node*       son,
    bool*             isAncestor
) {
    IF_ARG_NULL_RETURN(tree);
    IF_ARG_NULL_RETURN(father);
    IF_ARG_NULL_RETURN(son);
    IF_ARG_NULL_RETURN(isAncestor);

    *isAncestor = false;
    Node cur = *son;
    while (true) {
        if (cur.memBuffIndex == father->memBuffIndex) {
            *isAncestor = true;
            return SEMANTIC_CHECKER_STATUS_OK;
        }

        size_t parentInd = cur.parent;
        if (parentInd == 0)
            break;
        cur = *getSyntaxTreeNodePtr(tree, parentInd);
    }

    return SEMANTIC_CHECKER_STATUS_OK;
}

static SemanticCheckerErrors recursiveCheckForIdentficatorsScopes(
    SemanticChecker*  checker,
    const SyntaxTree* tree,
    size_t            curNodeInd
) {
    IF_ARG_NULL_RETURN(checker);

    if (!curNodeInd)
        return SEMANTIC_CHECKER_STATUS_OK;

    Node node   = *getSyntaxTreeNodePtr(tree, curNodeInd);
    Lexem lexem = node.lexem;

    size_t left = node.left;
    size_t right = node.right;

    if (lexem.type == IDENTIFICATOR_LEXEM_TYPE) {
        Identificator id = {};
        IF_ERR_RETURN(getIdentificatorByLexem(checker, &lexem, &id));
        if (id.declNodeInd != curNodeInd) { // that's not a declaration, that's a usage of some identificator
            bool isAncestor = false;
            IF_ERR_RETURN(isAncestorInSyntaxTree(tree, id.scopeNode, &node, &isAncestor));
            IF_NOT_COND_RETURN(isAncestor, SEMANTIC_CHECKER_IDENTIFICATOR_USAGE_OUTSIDE_ITS_SCOPE);
        }
    }

    IF_ERR_RETURN(recursiveCheckForIdentficatorsScopes(checker, tree, left));
    IF_ERR_RETURN(recursiveCheckForIdentficatorsScopes(checker, tree, right));

    return SEMANTIC_CHECKER_STATUS_OK;
}
