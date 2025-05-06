#include "../include/codeGenerator.hpp"
#include "../include/commonFileStart.hpp"

const size_t MAX_LINE_LEN = 1 << 10;

char* line = NULL;

CodeGeneratorErrors constructCodeGenerator(
    const char*         sourceFilePath,
    const char*         destFilePath,
    Dumper*             dumper,
    CodeGenerator*      generator
) {
    IF_ARG_NULL_RETURN(sourceFilePath);
    IF_ARG_NULL_RETURN(destFilePath);
    IF_ARG_NULL_RETURN(generator);

    *generator = {
        .sourceFilePath = sourceFilePath,
        .destFilePath   = destFilePath,
        .tree           = {},
        .dumper         = dumper
    };
    constructSyntaxTree(&generator->tree, generator->dumper);

    generator->checker = {};
    constructSemanticChecker(&generator->checker, &generator->tree);

    return CODE_GENERATOR_STATUS_OK;
}

CodeGeneratorErrors readCodeGeneratorSyntaxTreeFromFile(CodeGenerator* generator) {
    IF_ARG_NULL_RETURN(generator);

    readSyntaxTreeFromFile(&generator->tree, generator->sourceFilePath);
    buildTableOfIdentificators(&generator->checker);
    dumpTableOfIdentificators(&generator->checker);
    recursiveFindTinTout(&generator->checker, generator->checker.tree->root);

    return CODE_GENERATOR_STATUS_OK;
}

void clearLine() {
    for (int i = 0; i < MAX_LINE_LEN; ++i) {
        line[i] = '\0';
    }
}

#define ADD2BUFF(format, ...) \
    linePtr += snprintf(linePtr, MAX_LINE_LEN - (linePtr - line), format, ##__VA_ARGS__);

#define CLEAR_LINE() clearLine(), linePtr = line

CodeGeneratorErrors assemblerCodeForConst(const Node* node, char* linePtr) {
    IF_ARG_NULL_RETURN(node);
    IF_ARG_NULL_RETURN(linePtr);

    Lexem lexem = node->lexem;
    ADD2BUFF("push %.2f\n", lexem.doubleData);

    return CODE_GENERATOR_STATUS_OK;
}

#include "delimsAsmDefines.cpp"
#include "keywordsAsmDefines.cpp"
#include "operatorsAsmDefines.cpp"

CodeGeneratorErrors recursiveGenerationOfAssemblerCode(
    CodeGenerator*          generator,
    size_t                  curNodeInd,
    size_t                  depthInBlocksOfCode,
    FILE*                   file,
    Identificator           curFuncBody
);

CodeGeneratorErrors saveFuncLocalVariables(
    CodeGenerator*          generator,
    Node                    node,
    size_t                  depthInBlocksOfCode,
    FILE*                   file,
    Identificator           curFuncBody,
    char* linePtr
) {
    CLEAR_LINE();
//
//     for (size_t varInd = 0; varInd < curFuncBody.function.numOfLocalVars; ++varInd) {
//         size_t varArrInd = curFuncBody.function.arrOfLocalVars[varInd];
//         Identificator var = generator->checker.tableOfVars[varArrInd]; // TODO: add getter func
//         LOG_DEBUG_VARS(varArrInd, var.lexem.strRepr, var.lexem.type);
//         PUSH_VAR_IDENTIFICATOR(var.lexem);
//     }

    LOG_DEBUG_VARS("right subtree recursion");
    PRINT();
    CLEAR_LINE();
    GEN4LEFT();
    CLEAR_LINE();

    ADD_TABS();
    ADD2BUFF("push BX\n");
    ADD_TABS();
    ADD2BUFF("push %d\n", curFuncBody.function.numOfLocalVars);
    ADD_TABS();
    ADD2BUFF("add\n");
    ADD_TABS();
    ADD2BUFF("pop BX\n");
    PRINT();
    CLEAR_LINE();
}

CodeGeneratorErrors loadFuncLocalVariables(
    CodeGenerator*          generator,
    size_t                  depthInBlocksOfCode,
    FILE*                   file,
    Identificator           curFuncBody,
    char* linePtr
) {
    if (curFuncBody.type == INVALID_IDENTIFICATOR) // this is global scope
        return CODE_GENERATOR_STATUS_OK;

    CLEAR_LINE();
    ADD_TABS();
    ADD2BUFF("push BX\n");
    ADD_TABS();
    ADD2BUFF("push %d\n", curFuncBody.function.numOfLocalVars);
    ADD_TABS();
    ADD2BUFF("sub\n");
    ADD_TABS();
    ADD2BUFF("pop BX\n");

    // for (int varInd = curFuncBody.function.numOfLocalVars - 1; varInd >= 0; --varInd) {
    //     size_t varArrInd = curFuncBody.function.arrOfLocalVars[varInd];
    //     Identificator var = generator->checker.tableOfVars[varArrInd]; // TODO: add getter func
    //     LOG_DEBUG_VARS(varArrInd, var.lexem.strRepr, var.lexem.type);
    //     POP_VAR_IDENTIFICATOR(var.lexem);
    // }

    ADD2BUFF("\n");
    PRINT();
    CLEAR_LINE();
}

size_t numOfLabelsBefore = 0; // FIXME: мега костыль

CodeGeneratorErrors recursiveGenerationOfAssemblerCode(
    CodeGenerator*          generator,
    size_t                  curNodeInd,
    size_t                  depthInBlocksOfCode,
    FILE*                   file,
    Identificator           curFuncBody
) {
    IF_ARG_NULL_RETURN(generator);
    IF_ARG_NULL_RETURN(file);

    if (!curNodeInd)
        return CODE_GENERATOR_STATUS_OK;

    Node node = *getSyntaxTreeNodePtr(&generator->tree, curNodeInd);
    Lexem lexem = node.lexem;
    char* linePtr = line;
    //LOG_DEBUG_VARS(depthInBlocksOfCode);
    LOG_DEBUG_VARS(lexem.strRepr, curFuncBody.type);

    CLEAR_LINE();
    if (lexem.lexemSpecificName == KEYWORD_INT_LEXEM) { // not return only in case of func declaration
        Node left = *getSyntaxTreeNodePtr(&generator->tree, node.left);
        if (left.lexem.type != IDENTIFICATOR_LEXEM_TYPE)
            return CODE_GENERATOR_STATUS_OK;

        Identificator id = {};
        getIdentificatorByLexem(&generator->checker, &left.lexem, &id);
        if (id.type != FUNCTION_IDENTIFICATOR)
            return CODE_GENERATOR_STATUS_OK;
    }

    if (lexem.type == CONST_LEXEM_TYPE) {
        ADD_TABS();
        IF_ERR_RETURN(assemblerCodeForConst(&node, linePtr));
        PRINT();
        CLEAR_LINE();
        return CODE_GENERATOR_STATUS_OK;
    }

    // FIXME:
    // FIXME:
    // FIXME:
    if (lexem.type == IDENTIFICATOR_LEXEM_TYPE) {
        CLEAR_LINE();
        Identificator id = {};
        getIdentificatorByLexem(&generator->checker, &lexem, &id);
        size_t funcLabelInd = -1;
        if (id.declNodeInd == curNodeInd) { // declaration
            if (id.type == FUNCTION_IDENTIFICATOR) {
                ADD_TABS();
                funcLabelInd = ++numOfLabelsBefore;
                ADD2BUFF("jmp funcSafetyLabel%d:\n", funcLabelInd);
                ADD_TABS();
                LOG_DEBUG_VARS("func declaration");
                ADD2BUFF("%s:\n", id.lexem.strRepr);

                LOG_DEBUG_VARS(id.function.numOfArgs);
                for (size_t argInd = 0; argInd < id.function.numOfArgs; ++argInd) {
                    size_t varArrInd = id.function.argumentVars[argInd];
                    Identificator var = generator->checker.tableOfVars[varArrInd]; // TODO: add getter func
                    LOG_DEBUG_VARS(varArrInd, var.lexem.strRepr, var.lexem.type);
                    POP_VAR_IDENTIFICATOR(var.lexem);
                }

                PRINT();
                CLEAR_LINE();
                curFuncBody = id; // we enter function body
                LOG_DEBUG_VARS(curFuncBody.type);
            }
        } else {
            if (id.type == FUNCTION_IDENTIFICATOR) { // function call
                LOG_ERROR("i am function");
                LOG_DEBUG_VARS(curFuncBody.type);
                LOG_ERROR("callllll");
                IF_ERR_RETURN(saveFuncLocalVariables(
                    generator, node, depthInBlocksOfCode, file, curFuncBody, linePtr));

                ADD_TABS();
                ADD2BUFF("\n");

                ADD_TABS();
                ADD2BUFF("call %s:\n", id.lexem.strRepr);
                ADD_TABS();
                ADD2BUFF("\n");
                PRINT();
                CLEAR_LINE();

                IF_ERR_RETURN(loadFuncLocalVariables(
                    generator, depthInBlocksOfCode, file, curFuncBody, linePtr));

                ADD_TABS();
                ADD2BUFF("push AX\n");
                PRINT();
                CLEAR_LINE();

                return CODE_GENERATOR_STATUS_OK;
            } else { // variable usage
                LOG_ERROR("push var");
                PUSH_VAR_IDENTIFICATOR(lexem);

                PRINT();
                CLEAR_LINE();
            }
        }

        CLEAR_LINE();
        GEN4LEFT();
        GEN4RIGHT();

        if (id.declNodeInd == curNodeInd && id.type == FUNCTION_IDENTIFICATOR) {
            CLEAR_LINE();
            ADD_TABS();
            ADD2BUFF("funcSafetyLabel%d:\n", funcLabelInd);
            PRINT();
            CLEAR_LINE();
        }
        return CODE_GENERATOR_STATUS_OK;
    }

    #define GENERAL_LEXEM_DEF(_, specName, ...) \
        case specName:                          \
            specName##_ASM_DEF();               \
            break;                              \

    switch (lexem.lexemSpecificName) {
        #include "../../LexemsRealizations/include/codeGen/allLexems.hpp"
        default:
            assert(false);
    }

    #undef GENERAL_LEXEM_DEF

    CLEAR_LINE();

    return CODE_GENERATOR_STATUS_OK;
}

CodeGeneratorErrors generateAssemblerCodeFromSyntaxTree(CodeGenerator* generator) {
    IF_ARG_NULL_RETURN(generator);

    FILE* file = fopen(generator->destFilePath, "w");
    IF_NOT_COND_RETURN(file != NULL, CODE_GENERATOR_FILE_OPENING_ERROR);

    line = (char*)calloc(MAX_LINE_LEN, sizeof(char));
    IF_NOT_COND_RETURN(line != NULL, CODE_GENERATOR_MEMORY_ALLOCATION_ERROR);

    Identificator id = {};
    id.type = INVALID_IDENTIFICATOR;
    recursiveGenerationOfAssemblerCode(generator, generator->tree.root, 0, file, id);

    clearLine();
    char* linePtr = line;
    ADD2BUFF("call main:\n");
    PRINT();

    fclose(file);

    return CODE_GENERATOR_STATUS_OK;
}

CodeGeneratorErrors destructCodeGenerator(CodeGenerator* generator) {
    IF_ARG_NULL_RETURN(generator);

    destructSyntaxTree(&generator->tree);
    destructSemanticChecker(&generator->checker);

    return CODE_GENERATOR_STATUS_OK;
}

