#include <ctype.h>
#include <errno.h>

#include "../include/errorsHandler.hpp"
#include "../include/lexemsRealizations.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, LEXEMS_REALIZATIONS_INVALID_ARGUMENT, getLexemsRealizationsErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getLexemsRealizationsErrorMessage, LEXEMS_REALIZATIONS_STATUS_OK);

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getLexemsRealizationsErrorMessage)\

const int baseLineNum = __LINE__;

// BRUUUUUUH
#define GENERAL_LEXEM_DEF(lexemType, enumName, repr) \
    {.type=lexemType, .strRepr=repr, {.lexemSpecificName = enumName}},

const Lexem allLexemsArr[] = {
    {.type=INVALID_LEXEM_TYPE, .strRepr = NULL, {.lexemSpecificName = INVALID_LEXEM}},
    #include "../include/codeGen/allLexems.hpp"
};



#define GENERAL_LEXEM_DEF(lexemType, enumName, repr) \
    #enumName,

const char* allLexemSpecNames[] = {
    #include "../include/codeGen/allLexems.hpp"
};

#undef GENERAL_LEXEM_DEF

const size_t LEXEM_ARR_SIZE = sizeof(allLexemsArr) / sizeof(*allLexemsArr);
const size_t ALL_LEXEMS_SPEC_NAMES = sizeof(allLexemSpecNames) / sizeof(*allLexemSpecNames);

// TODO: super slow
LexemsRealizationsErrors initLexemWithString(const char* line, Lexem* lexem) {
    IF_ARG_NULL_RETURN(line);
    IF_ARG_NULL_RETURN(lexem);

    LOG_DEBUG_VARS(LEXEM_ARR_SIZE, line);
    for (int i = 1; i < LEXEM_ARR_SIZE; ++i) {
        bool isSame = strcmp(allLexemsArr[i].strRepr, line) == 0;
        //LOG_DEBUG_VARS(i, allLexemsArr[i].strRepr, line, isSame);
        if (isSame) {
            *lexem = allLexemsArr[i];
            return LEXEMS_REALIZATIONS_STATUS_OK;
        }
    }



    // maybe this is const (number or literal) or identificator

    assert(strlen(line) > 0);
    // ASK: cringe, how to do properly
    if (isdigit(line[0]) || line[0] == '-') { // this is const number
        char* endPtr = NULL;
        errno = 0;
        lexem->doubleData = strtod(line, &endPtr);
        assert(errno == 0);
        lexem->type = CONST_LEXEM_TYPE;
    } else {
        // TODO: add identificator to table of names
        lexem->lexemSpecificName = INVALID_LEXEM;
        lexem->strRepr = (char*)calloc(strlen(line) + 1, sizeof(char));
        IF_NOT_COND_RETURN(lexem->strRepr != NULL, LEXEMS_REALIZATIONS_MEMORY_ALLOCATION_ERROR);
        strcpy(lexem->strRepr, line);
        //assert(false); // not implemented yet
        lexem->type = IDENTIFICATOR_LEXEM_TYPE;
    }

    return LEXEMS_REALIZATIONS_STATUS_OK;
}

LexemsRealizationsErrors isCharLexemDelim(const char ch, bool* isDelim) {
    IF_ARG_NULL_RETURN(isDelim);

    *isDelim = false;
    char line[] = { ch, '\0' };
    for (int i = 1; i < LEXEM_ARR_SIZE; ++i) {
        //LOG_DEBUG_VARS(i, allLexemsArr[i].strRepr);
        bool isSame = strcmp(allLexemsArr[i].strRepr, line) == 0;
        if (isSame && allLexemsArr[i].type == DELIM_LEXEM_TYPE) { // second operand is just to be sure, it should not necessary as we check strings for equality
            *isDelim = true;
            return LEXEMS_REALIZATIONS_STATUS_OK;
        }
    }

    return LEXEMS_REALIZATIONS_STATUS_OK;
}

const char* getLexemTypeString(LexemType type) {
    #define helpDef(x) case x: return #x
    switch (type) {
        helpDef(INVALID_LEXEM_TYPE);
        helpDef(OPERATOR_LEXEM_TYPE);
        helpDef(KEYWORD_LEXEM_TYPE);
        helpDef(CONST_LEXEM_TYPE);
        helpDef(IDENTIFICATOR_LEXEM_TYPE);
        helpDef(DELIM_LEXEM_TYPE);
        default:
            return "INVALID_LEXEM_TYPE";
    }
}

static LexemsRealizationsErrors initLexemFromFileFormat(LexemType type, const char* line, Lexem* lexem) {
    IF_ARG_NULL_RETURN(line);
    IF_ARG_NULL_RETURN(lexem);

    lexem->type    = type;
    char* endPtr   = NULL;
    char* strRepr  = NULL;
    size_t lineLen = strlen(line);
    errno = 0;
    LOG_DEBUG_VARS(type, line);
    switch (type) {
        case CONST_LEXEM_TYPE:
            lexem->doubleData = strtod(line, &endPtr);
            assert(errno == 0);

        case IDENTIFICATOR_LEXEM_TYPE:
            strRepr = (char*)calloc(lineLen + 1, sizeof(char));
            IF_NOT_COND_RETURN(strRepr != NULL, LEXEMS_REALIZATIONS_MEMORY_ALLOCATION_ERROR);
            strcpy(strRepr, line);
            strRepr[lineLen - 1] = '\0';
            lexem->strRepr = strRepr;
            break;

        case KEYWORD_LEXEM_TYPE:
        case DELIM_LEXEM_TYPE:
        case OPERATOR_LEXEM_TYPE:
            lexem->lexemSpecificName = (Lexems)strtol(line, &endPtr, 10);
            lexem->strRepr = allLexemsArr[lexem->lexemSpecificName].strRepr;
            assert(errno == 0);
            break;

        case INVALID_LEXEM_TYPE:
        default:
            assert(false); // add error
            break;
    }

    return LEXEMS_REALIZATIONS_STATUS_OK;
}

// reads 2 lines from file and initializes given lexem
LexemsRealizationsErrors readLexemFromFile(FILE* file, Lexem* lexem,
                                           char* lineBuffer, const size_t maxLineLen) {
    IF_ARG_NULL_RETURN(file);
    IF_ARG_NULL_RETURN(lexem);
    IF_ARG_NULL_RETURN(lineBuffer);

    fgets(lineBuffer, maxLineLen, file);
    assert(lineBuffer != NULL);

    char* endPtr = 0;
    errno = 0;
    LexemType lexemType = (LexemType)strtol(lineBuffer, &endPtr, 10); // ASK: bad cast?
    LOG_DEBUG_VARS(lineBuffer, lexemType);
    assert(errno == 0);
    //LOG_DEBUG_VARS(lineBuffer, lexemType);

    fgets(lineBuffer, maxLineLen, file);
    assert(lineBuffer != NULL);

    //LOG_DEBUG_VARS(lineBuffer);
    initLexemFromFileFormat(lexemType, lineBuffer, lexem);
    LOG_DEBUG_VARS(lexem->lexemSpecificName, lexem->doubleData);

    return LEXEMS_REALIZATIONS_STATUS_OK;
}

LexemsRealizationsErrors saveLexemToFile(FILE* file, const Lexem* lexem) {
    IF_ARG_NULL_RETURN(file);
    IF_ARG_NULL_RETURN(lexem);

    fprintf(file, "%d\n", lexem->type);
    switch (lexem->type) {
        case CONST_LEXEM_TYPE:
            fprintf(file, "%.10f", lexem->doubleData);
            break;
        case IDENTIFICATOR_LEXEM_TYPE:
            fprintf(file, "%s", lexem->strRepr);
            break;
        case DELIM_LEXEM_TYPE:
        case KEYWORD_LEXEM_TYPE:
        case OPERATOR_LEXEM_TYPE:
            fprintf(file, "%d", lexem->lexemSpecificName); // general lexem id
            break;
        default:
            // invalid lexem type
            assert(false);
            break;
    }
    fprintf(file, "\n");

    return LEXEMS_REALIZATIONS_STATUS_OK;
}

LexemsRealizationsErrors getLexemDebugString(const Lexem* lexem, char** result) {
    IF_ARG_NULL_RETURN(lexem);
    IF_ARG_NULL_RETURN(result);

    const size_t DBG_STRING_LEN = 20;
    *result = (char*)calloc(DBG_STRING_LEN, sizeof(char));
    IF_NOT_COND_RETURN(*result != NULL, LEXEMS_REALIZATIONS_MEMORY_ALLOCATION_ERROR);

    switch (lexem->type) {
        case OPERATOR_LEXEM_TYPE:
        case DELIM_LEXEM_TYPE:
        case KEYWORD_LEXEM_TYPE:
            strcpy(*result, lexem->strRepr);
            break;
        case CONST_LEXEM_TYPE:
            snprintf(*result, DBG_STRING_LEN, "%.2f", lexem->doubleData);
            break;
        case IDENTIFICATOR_LEXEM_TYPE:
            strcpy(*result, lexem->strRepr);
            //LOG_DEBUG_VARS(lexem->strRepr, *result);
            break;
        default:
            assert(false); // TODO: unknown lexem type
    }

    return LEXEMS_REALIZATIONS_STATUS_OK;
}

LexemsRealizationsErrors getLexemDataString(const Lexem* lexem, const Lexem2stringSettings* settings, char** line) {
    IF_ARG_NULL_RETURN(lexem);
    IF_ARG_NULL_RETURN(settings);
    IF_ARG_NULL_RETURN(line);

    const size_t MAX_LINE_LEN = 50;
    *line = (char*)calloc(MAX_LINE_LEN, sizeof(char));
    IF_NOT_COND_RETURN(*line != NULL, LEXEMS_REALIZATIONS_MEMORY_ALLOCATION_ERROR);

    char* linePtr = *line;

    #define ADD2BUFF(format, ...) \
        linePtr += snprintf(linePtr, MAX_LINE_LEN - (linePtr - *line), format, __VA_ARGS__)

    const char* type = getLexemTypeString(lexem->type);
    if (settings->isBracketsNeeded)  ADD2BUFF("%s", "(");
    if (settings->isLexemTypeNeeded) ADD2BUFF(" %s ", type);
    char* debugStr = NULL;
    IF_ERR_RETURN(getLexemDebugString(lexem, &debugStr));
    ADD2BUFF(" %s ", debugStr);
    //LOG_DEBUG_VARS(linePtr);
    FREE(debugStr);
    if (settings->isBracketsNeeded)  ADD2BUFF("%s", ")");

    return LEXEMS_REALIZATIONS_STATUS_OK;
}
