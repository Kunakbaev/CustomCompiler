#include "commonDefines.cpp"

#define DELIMS_OPEN_CURLY_BRACKET_LEXEM_ASM_DEF()           \
    do {                                                    \
        ++depthInBlocksOfCode;                              \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
    } while (0)

#define DELIMS_CLOSE_CURLY_BRACKET_LEXEM_ASM_DEF()          \
    do {                                                    \
        --depthInBlocksOfCode;                              \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
    } while (0)

#define DELIMS_SEMICOLON_LEXEM_ASM_DEF()                    \
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        ADD2BUFF("\n");                                     \
        PRINT();                                            \
    } while (0)

#define DELIMS_OPEN_SIMPLE_BRACKET_LEXEM_ASM_DEF()\
    do {                                                    \
        GEN4LEFT();                                         \
        GEN4RIGHT();                                        \
        PRINT();                                            \
    } while (0)



#define DELIMS_CLOSE_SIMPLE_BRACKET_LEXEM_ASM_DEF()
#define DELIMS_COMMA_LEXEM_ASM_DEF()

