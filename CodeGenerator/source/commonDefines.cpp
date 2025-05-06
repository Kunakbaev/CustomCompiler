#define GEN4LEFT() \
    IF_ERR_RETURN(recursiveGenerationOfAssemblerCode(generator, node.left , depthInBlocksOfCode, file, curFuncBody))

#define GEN4RIGHT() \
    IF_ERR_RETURN(recursiveGenerationOfAssemblerCode(generator, node.right, depthInBlocksOfCode, file, curFuncBody))

#define PRINT() fprintf(file, "%s", line)

#define ADD_TABS()                                          \
    do {                                                    \
        for (int i = 0; i < depthInBlocksOfCode; ++i)       \
            ADD2BUFF("\t");                                 \
    } while (0)

#define PUSH_VAR_IDENTIFICATOR(lex) \
    do {                        \
        Identificator curId = {};\
        getIdentificatorByLexem(&generator->checker, &lex, &curId);\
        ADD_TABS();\
        ADD2BUFF("push [BX + %d]\n", curId.localVarInd);\
    } while (0)

// FIXME: copy paste
#define POP_VAR_IDENTIFICATOR(lex) \
    do {                        \
        Identificator curId = {};\
        getIdentificatorByLexem(&generator->checker, &lex, &curId);\
        ADD_TABS();\
        ADD2BUFF("pop [BX + %d]\n", curId.localVarInd);\
    } while (0)
