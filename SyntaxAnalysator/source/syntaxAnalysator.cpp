#include <errno.h>

#include "../include/commonFileStart.hpp"


const size_t MAX_NUM_OF_LINES_IN_INPUT = 1 << 10;
const size_t MAX_LINE_LEN              = 1 << 6;


SyntaxAnalysatorErrors constructSyntaxAnalysator(
    const char*       sourceFilePath,
    const char*       destFilePath,
    SyntaxAnalysator* analysator,
    Dumper*           dumper
) {
    IF_ARG_NULL_RETURN(sourceFilePath);
    IF_ARG_NULL_RETURN(destFilePath);
    IF_ARG_NULL_RETURN(analysator);

    *analysator = {
        .sourceFilePath = sourceFilePath,
        .destFilePath   = destFilePath,
        .arrOfLexems    = NULL,
        .lenOfArr       = 0,
        .tree           = {},
        .dumper         = dumper
    };

    constructSyntaxTree(&analysator->tree, analysator->dumper);

    return SYNTAX_ANALYSATOR_STATUS_OK;
}

size_t getNumOfLines(FILE* file) {
    assert(file != NULL);

    size_t ch = '?';
    size_t numOfLines = 0;
    while ((ch = fgetc(file)) != EOF) {
        numOfLines += ch == '\n';
    }
    rewind(file);

    return numOfLines; // ASK: +1?
}

SyntaxAnalysatorErrors readArrOfLexemsFromFile(SyntaxAnalysator* analysator) {
    IF_ARG_NULL_RETURN(analysator);

    FILE* file = fopen(analysator->sourceFilePath, "r");
    IF_NOT_COND_RETURN(file != NULL, SYNTAX_ANALYSATOR_FILE_OPENING_ERROR);

    size_t numOfLines = getNumOfLines(file);
    assert(numOfLines % 3 == 0);
    assert(numOfLines > 0);

    analysator->lenOfArr = numOfLines / 3;
    analysator->arrOfLexems = (Lexem*)calloc(analysator->lenOfArr, sizeof(Lexem));
    IF_NOT_COND_RETURN(analysator->arrOfLexems != NULL,
                       SYNTAX_ANALYSATOR_MEMORY_ALLOCATION_ERROR);

    char lineBuffer[MAX_NUM_OF_LINES_IN_INPUT] = {};
    for (size_t elemInd = 0; elemInd < analysator->lenOfArr; ++elemInd) {
        Lexem* lexem = &analysator->arrOfLexems[elemInd];
        LEXEMS_REALIZATIONS_ERR_CHECK(readLexemFromFile(file, lexem, lineBuffer, MAX_LINE_LEN));
        fgets(lineBuffer, MAX_NUM_OF_LINES_IN_INPUT, file); // reads empty line that separates 2 different lexems
    }

    return SYNTAX_ANALYSATOR_STATUS_OK;
}

SyntaxAnalysatorErrors dumpSyntaxAnalysatorArrOfLexems(const SyntaxAnalysator* analysator) {
    IF_ARG_NULL_RETURN(analysator);

    LOG_DEBUG_VARS("---------------------");
    LOG_DEBUG_VARS("syntax analysator array of lexems:");
    for (size_t arrInd = 0; arrInd < analysator->lenOfArr; ++arrInd) {
        Lexem lexem = analysator->arrOfLexems[arrInd];

        char* lexemDbgStr = NULL;
        getLexemDebugString(&lexem, &lexemDbgStr);
        const char* type = getLexemTypeString(lexem.type);
        LOG_DEBUG_VARS(lexem.type, lexem.lexemSpecificName, type, lexemDbgStr, lexem.doubleData);
        FREE(lexemDbgStr);
    }
    LOG_DEBUG_VARS("---------------------");

    return SYNTAX_ANALYSATOR_STATUS_OK;
}

SyntaxAnalysatorErrors dumpSyntaxAnalysatorTreeInConsole(const SyntaxAnalysator* analysator) {
    IF_ARG_NULL_RETURN(analysator);

    dumpSyntaxTreeInConsole(&analysator->tree);

    return SYNTAX_ANALYSATOR_STATUS_OK;
}

#include "generateSyntaxTree.cpp"

SyntaxAnalysatorErrors saveAnalysatorSyntaxTree2File(SyntaxAnalysator* analysator) {
    IF_ARG_NULL_RETURN(analysator);

    saveSyntaxTree2File(&analysator->tree, analysator->destFilePath);

    return SYNTAX_ANALYSATOR_STATUS_OK;
}

SyntaxAnalysatorErrors destructSyntaxAnalysator(SyntaxAnalysator* analysator) {
    IF_ARG_NULL_RETURN(analysator);

    destructSyntaxTree(&analysator->tree);
    FREE(analysator->arrOfLexems);
    analysator->lenOfArr = 0;

    return SYNTAX_ANALYSATOR_STATUS_OK;
}
