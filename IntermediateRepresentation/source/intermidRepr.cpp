
#include "intermidRepr.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, INTERMID_REPR_INVALID_ARGUMENT, getIntermidReprErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getIntermidReprErrorMessage, INTERMID_REPR_STATUS_OK);

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getIntermidReprErrorMessage)\

static IntermidReprErrors getIntermidReprFromTreeRec(
    const SyntaxTree* tree,
    size_t            curNodeInd,
    IntermidRepr*     intermidRepr
) {
    IF_ARG_NULL_RETURN(tree);
    IF_ARG_NULL_RETURN(intermidRepr);

    Node* node  = getSyntaxTreeNodePtr(tree, curNodeInd);
    Lexem lexem = node->lexem;

    IF_NOT_COND_RETURN(lexem.type != INVALID_LEXEM_TYPE,
                       INTERMID_REPR_INVALID_ARGUMENT);

    switch (lexem.type) {
        
    }

    IF_ERR_RETURN(getIntermidReprFromTreeRec(tree, node->left,  intermidRepr));
    IF_ERR_RETURN(getIntermidReprFromTreeRec(tree, node->right, intermidRepr));

    return INTERMID_REPR_STATUS_OK;
}

IntermidReprErrors constructIntermidReprFromSyntaxTree(
    const SyntaxTree* tree,
    IntermidRepr*     intermidRepr
) {
    IF_ARG_NULL_RETURN(tree);
    IF_ARG_NULL_RETURN(intermidRepr);

    

    return INTERMID_REPR_STATUS_OK;
}
