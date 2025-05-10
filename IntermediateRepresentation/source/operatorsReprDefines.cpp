#include "commonDefines.cpp"

#define OPERATOR_ADD_LEXEM_REPR_DEF()                        \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_COMMAND_NO_ARGS(ADD_INTERMID_REPR_CMD);         \
    } while (0)

#define OPERATOR_SUB_LEXEM_REPR_DEF()                        \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_COMMAND_NO_ARGS(SUB_INTERMID_REPR_CMD);         \
    } while (0)

#define OPERATOR_MUL_LEXEM_REPR_DEF()                        \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_COMMAND_NO_ARGS(MUL_INTERMID_REPR_CMD);         \
    } while (0)

#define OPERATOR_DIV_LEXEM_REPR_DEF()                        \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_COMMAND_NO_ARGS(DIV_INTERMID_REPR_CMD);         \
    } while (0)

#define OPERATOR_ASSIGN_LEXEM_REPR_DEF()                                  \
    do {                                                                 \
        GEN4RIGHT();                                                     \
        Node left = *getSyntaxTreeNodePtr(&intermidRepr->tree, node->left);  \
        LOG_DEBUG_VARS("assign pop var", left.lexem.strRepr);\
        size_t varInd = getLocalVariableIndex(function, left.lexem.strRepr); \
        ADD_COMMAND(POP_VAR_INTERMID_REPR_CMD, variableInd, varInd); \
    } while (0)

#define OPERATOR_LESS_LEXEM_REPR_DEF()                       \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_COMMAND_NO_ARGS(IS_LESS_INTERMID_REPR_CMD);     \
    } while (0)

#define OPERATOR_LESS_OR_EQUAL_LEXEM_REPR_DEF()                    \
    do {                                                          \
        GEN4LEFT();                                               \
        GEN4RIGHT();                                              \
        ADD_COMMAND_NO_ARGS(IS_LESS_OR_EQ_INTERMID_REPR_CMD);     \
    } while (0)

#define OPERATOR_GREATER_LEXEM_REPR_DEF()                    \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_COMMAND_NO_ARGS(IS_GREATER_INTERMID_REPR_CMD);  \
    } while (0)

#define OPERATOR_GREATER_OR_EQUAL_LEXEM_REPR_DEF()                   \
    do {                                                            \
        GEN4LEFT();                                                 \
        GEN4RIGHT();                                                \
        ADD_COMMAND_NO_ARGS(IS_GREATER_OR_EQ_INTERMID_REPR_CMD);    \
    } while (0)

#define OPERATOR_IS_EQUAL_LEXEM_REPR_DEF()                   \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_COMMAND_NO_ARGS(IS_EQ_INTERMID_REPR_CMD);       \
    } while (0)

#define OPERATOR_LOGIC_AND_LEXEM_REPR_DEF()                  \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_COMMAND_NO_ARGS(AND_INTERMID_REPR_CMD);         \
    } while (0)

#define OPERATOR_LOGIC_OR_LEXEM_REPR_DEF()                   \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_COMMAND_NO_ARGS(OR_INTERMID_REPR_CMD);          \
    } while (0)

