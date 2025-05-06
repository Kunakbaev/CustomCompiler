#include "commonDefines.cpp"

#define OPERATOR_ADD_LEXEM_ASM_DEF()                        \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_TABS();                                         \
        ADD2BUFF("add\n");                                  \
        PRINT();                                            \
    } while (0)

#define OPERATOR_SUB_LEXEM_ASM_DEF()                        \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_TABS();                                         \
        ADD2BUFF("sub\n");                                  \
        PRINT();                                            \
    } while (0)

#define OPERATOR_MUL_LEXEM_ASM_DEF()                        \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_TABS();                                         \
        ADD2BUFF("mul\n");                                  \
        PRINT();                                            \
    } while (0)

#define OPERATOR_DIV_LEXEM_ASM_DEF()                        \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_TABS();                                         \
        ADD2BUFF("div\n");                                  \
        PRINT();                                            \
    } while (0)

#define OPERATOR_ASSIGN_LEXEM_ASM_DEF()                                  \
    do {                                                                 \
        GEN4RIGHT();                                                     \
        Node left = *getSyntaxTreeNodePtr(&generator->tree, node.left);  \
        LOG_DEBUG_VARS("assing pop var", left.lexem.strRepr);\
        POP_VAR_IDENTIFICATOR(left.lexem);                               \
        PRINT();                                                         \
    } while (0)

#define OPERATOR_LESS_LEXEM_ASM_DEF()                       \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_TABS();                                         \
        ADD2BUFF("lessCmp\n");                              \
        PRINT();                                            \
    } while (0)

#define OPERATOR_LESS_OR_EQUAL_LEXEM_ASM_DEF()              \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_TABS();                                         \
        ADD2BUFF("lessOrEqualCmp\n");                       \
        PRINT();                                            \
    } while (0)

#define OPERATOR_GREATER_LEXEM_ASM_DEF()                    \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_TABS();                                         \
        ADD2BUFF("greaterCmp\n");                           \
        PRINT();                                            \
    } while (0)

#define OPERATOR_GREATER_OR_EQUAL_LEXEM_ASM_DEF()           \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_TABS();                                         \
        ADD2BUFF("greaterOrEqualCmp\n");                    \
        PRINT();                                            \
    } while (0)

#define OPERATOR_IS_EQUAL_LEXEM_ASM_DEF()                   \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_TABS();                                         \
        ADD2BUFF("isEqualCmp\n");                           \
        PRINT();                                            \
    } while (0)

#define OPERATOR_LOGIC_AND_LEXEM_ASM_DEF()                  \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_TABS();                                         \
        ADD2BUFF("logicAnd\n");                             \
        PRINT();                                            \
    } while (0)

#define OPERATOR_LOGIC_OR_LEXEM_ASM_DEF()                   \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD_TABS();                                         \
        ADD2BUFF("logicOr\n");                              \
        PRINT();                                            \
    } while (0)

