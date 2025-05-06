#include "commonDefines.cpp"
//
// #define KEYWORD_IF_LEXEM_ASM_DEF()
// #define KEYWORD_WHILE_LEXEM_ASM_DEF()
//

#define KEYWORD_INT_LEXEM_ASM_DEF() \
    do {                                \
        GEN4LEFT();\
        GEN4RIGHT();\
    } while (0)

#define KEYWORD_IF_LEXEM_ASM_DEF()                          \
    do {                                                    \
        GEN4LEFT();                                         \
        \
        CLEAR_LINE();\
        ADD_TABS();\
        ADD2BUFF("push 0\n");\
        ADD_TABS();\
        ADD2BUFF("je endif%d:\n", ++numOfLabelsBefore);\
        PRINT();\
        \
        GEN4RIGHT();                                        \
        \
        CLEAR_LINE();\
        ADD_TABS();\
        ADD2BUFF("endif%d:\n", numOfLabelsBefore);\
        PRINT();\
    } while (0)

#define KEYWORD_WHILE_LEXEM_ASM_DEF()                       \
    do {                                                    \
        ADD_TABS();                                         \
        ADD2BUFF("whileStart%d:\n", ++numOfLabelsBefore);                          \
        PRINT();                                            \
                                                            \
        GEN4LEFT();                                         \
                                                            \
        CLEAR_LINE();                                       \
        ADD_TABS();                                         \
        ADD2BUFF("push 0\n");                               \
        ADD_TABS();                                         \
        ADD2BUFF("je endWhile%d:\n", numOfLabelsBefore);                         \
        PRINT();                                            \
                                                            \
        GEN4RIGHT();                                        \
                                                            \
        CLEAR_LINE();                                       \
        ADD_TABS();                                         \
        ADD2BUFF("jmp whileStart%d:\n", numOfLabelsBefore);                      \
        ADD_TABS();                                         \
        ADD2BUFF("endWhile%d:\n", numOfLabelsBefore);                            \
        PRINT();                                            \
    } while (0)

static CodeGeneratorErrors recursiveGenerationOfCodeForInputOrOutput(
    CodeGenerator*          generator,
    size_t                  curNodeInd,
    size_t                  depthInBlocksOfCode,
    FILE*                   file,
    Identificator           id,
    bool                    isOutput
) {
    IF_ARG_NULL_RETURN(generator);
    IF_ARG_NULL_RETURN(file);

    if (!curNodeInd)
        return CODE_GENERATOR_STATUS_OK;

    Node node = *getSyntaxTreeNodePtr(&generator->tree, curNodeInd);
    Lexem lexem = node.lexem;
    char* linePtr = line;
    //LOG_DEBUG_VARS(depthInBlocksOfCode);
    LOG_DEBUG_VARS(lexem.strRepr);

    CLEAR_LINE();
    if (lexem.type == IDENTIFICATOR_LEXEM_TYPE) {
        Identificator curId = {};
        getIdentificatorByLexem(&generator->checker, &lexem, &curId);
        if (isOutput) {
            PUSH_VAR_IDENTIFICATOR(lexem);
            ADD_TABS();
            ADD2BUFF("out\n");
        } else {
            ADD_TABS();
            ADD2BUFF("in\n");
            ADD_TABS();
            POP_VAR_IDENTIFICATOR(lexem);
        }
        PRINT();
        return CODE_GENERATOR_STATUS_OK;
    }

    if (lexem.type == CONST_LEXEM_TYPE) {
        if (!isOutput)
            assert(isOutput || lexem.type != CONST_LEXEM_TYPE); // assert(false)
        ADD_TABS();
        ADD2BUFF("push %s\n", lexem.strRepr);
        ADD_TABS();
        ADD2BUFF("out\n");
        PRINT();
        return CODE_GENERATOR_STATUS_OK;
    }

    IF_ERR_RETURN(recursiveGenerationOfCodeForInputOrOutput(
        generator, node.left,  depthInBlocksOfCode, file, id, isOutput));
    IF_ERR_RETURN(recursiveGenerationOfCodeForInputOrOutput(
        generator, node.right, depthInBlocksOfCode, file, id, isOutput));
    return CODE_GENERATOR_STATUS_OK;
}

#define KEYWORD_INPUT_LEXEM_ASM_DEF()                       \
    do {                                                    \
        IF_ERR_RETURN(recursiveGenerationOfCodeForInputOrOutput(\
            generator, curNodeInd, depthInBlocksOfCode, file, curFuncBody, false));\
    } while (0)

#define KEYWORD_OUTPUT_LEXEM_ASM_DEF()                       \
    do {                                                    \
        IF_ERR_RETURN(recursiveGenerationOfCodeForInputOrOutput(\
            generator, curNodeInd, depthInBlocksOfCode, file, curFuncBody, true));\
    } while (0)

#define KEYWORD_RETURN_LEXEM_ASM_DEF()                      \
    do {                                                    \
        GEN4LEFT();\
        GEN4RIGHT();\
        ADD_TABS();\
        ADD2BUFF("pop AX\n");\
        ADD_TABS();\
        ADD2BUFF("ret\n");\
        PRINT();\
    } while (0)
