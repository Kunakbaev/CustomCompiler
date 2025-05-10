#include "commonDefines.cpp"
//
// #define KEYWORD_IF_LEXEM_REPR_DEF()
// #define KEYWORD_WHILE_LEXEM_REPR_DEF()
//

#define KEYWORD_INT_LEXEM_REPR_DEF() \
    do {                                \
        GEN4LEFT();\
        GEN4RIGHT();\
    } while (0)

#define KEYWORD_IF_LEXEM_REPR_DEF()                                         \
    do {                                                                   \
        GEN4LEFT();                                                        \
        ADD_COMMAND(PUSH_NUM_INTERMID_REPR_CMD, doubleNumber, 0);              \ 
        ADD_COMMAND(IF_EQ_JMP_INTERMID_REPR_CMD, jumpDestLabelInd, 0);        \
        IntermidReprElement* jmpCommand = function->listTail;              \
        GEN4RIGHT();                                                       \
        jmpCommand->command.argument.jumpDestLabelInd = numOfLabelsBefore; \
        ADD_LABEL();\
    } while (0)

#define KEYWORD_WHILE_LEXEM_REPR_DEF()                                                      \
    do {                                                                                   \
        size_t loopStartLabelInd = numOfLabelsBefore;                                      \
        ADD_LABEL();                                                                       \ 
        GEN4LEFT();                                                                        \
        ADD_COMMAND(PUSH_NUM_INTERMID_REPR_CMD, doubleNumber, 0);                              \ 
        ADD_COMMAND(IF_EQ_JMP_INTERMID_REPR_CMD, jumpDestLabelInd, 0);                        \
        IntermidReprElement* jmpCommand = function->listTail;                              \
        GEN4RIGHT();                                                                       \
        jmpCommand->command.argument.jumpDestLabelInd = numOfLabelsBefore;                 \
        ADD_COMMAND(UNCOND_JMP_INTERMID_REPR_CMD, jumpDestLabelInd, loopStartLabelInd);    \
        ADD_LABEL();                                                                       \
    } while (0)

static IntermidReprErrors recursiveGenerationOfCommandsForInputOrOutput(
    const IntermidRepr*     intermidRepr,
    FunctionRepr*           function,
    size_t                  curNodeInd,
    bool                    isOutput,
    size_t                  depthInBlocksOfCode
) {
    IF_ARG_NULL_RETURN(intermidRepr);
    IF_ARG_NULL_RETURN(function);

    if (!curNodeInd)
        return INTERMID_REPR_STATUS_OK;

    Node node = *getSyntaxTreeNodePtr(&intermidRepr->tree, curNodeInd);
    Lexem lexem = node.lexem;
    LOG_DEBUG_VARS(lexem.strRepr, lexem.lexemSpecificName, isOutput);
    IF_NOT_COND_RETURN(isOutput || lexem.type != CONST_LEXEM_TYPE,
                       INTERMID_REPR_INVALID_ARGUMENT);

    bool isLeaf = lexem.type == IDENTIFICATOR_LEXEM_TYPE ||
                  lexem.type ==         CONST_LEXEM_TYPE;

    if (!isOutput && isLeaf) {
        ADD_COMMAND_NO_ARGS(INPUT_INTERMID_REPR_CMD);
    }

    if (lexem.type == IDENTIFICATOR_LEXEM_TYPE) {
        if (isOutput) {
            size_t varInd = getLocalVariableIndex(function, lexem.strRepr);
            ADD_COMMAND(PUSH_VAR_INTERMID_REPR_CMD, variableInd, varInd);
        } else {
            size_t varInd = getLocalVariableIndex(function, lexem.strRepr);
            ADD_COMMAND(POP_VAR_INTERMID_REPR_CMD, variableInd, varInd);
        }
    }

    if (lexem.type == CONST_LEXEM_TYPE) {
        ADD_COMMAND(PUSH_NUM_INTERMID_REPR_CMD, doubleNumber, lexem.doubleData);
    }

    if (isOutput && isLeaf) {
        ADD_COMMAND_NO_ARGS(OUTPUT_INTERMID_REPR_CMD);
    }

    if (!isLeaf) {
        IF_ERR_RETURN(recursiveGenerationOfCommandsForInputOrOutput(
            intermidRepr, function, node.left, isOutput, depthInBlocksOfCode));
        IF_ERR_RETURN(recursiveGenerationOfCommandsForInputOrOutput(
            intermidRepr, function, node.right, isOutput, depthInBlocksOfCode));
    }

    return INTERMID_REPR_STATUS_OK;
}

#define KEYWORD_INPUT_LEXEM_REPR_DEF()                                   \
    do {                                                                \
        IF_ERR_RETURN(recursiveGenerationOfCommandsForInputOrOutput(    \
            intermidRepr, function, curNodeInd, false, depthInBlocksOfCode));                \
    } while (0)

#define KEYWORD_OUTPUT_LEXEM_REPR_DEF()                                  \
    do {                                                                \
        IF_ERR_RETURN(recursiveGenerationOfCommandsForInputOrOutput(        \
            intermidRepr, function, curNodeInd, true, depthInBlocksOfCode));                \
    } while (0)

#define KEYWORD_RETURN_LEXEM_REPR_DEF()                                  \
    do {                                                                \
        GEN4LEFT();                                                     \
        GEN4RIGHT();                                                    \
        ADD_COMMAND_NO_ARGS(LEAVE_INTERMID_REPR_CMD);                   \
    } while (0)
