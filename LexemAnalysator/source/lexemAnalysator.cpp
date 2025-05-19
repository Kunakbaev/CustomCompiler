#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "../include/lexemAnalysator.hpp"
//#include "../include/commonFileStart.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, LEXEM_ANALYSATOR_INVALID_ARGUMENT, getLexemAnalysatorErrorMessage)

#define IF_ERR_RETURN(error) \
    do {\
        COMMON_IF_ERR_RETURN(error, getLexemAnalysatorErrorMessage, LEXEM_ANALYSATOR_STATUS_OK);\
        /*DEBUG_VARS_TO_DUMPER_ALL_LOGS_FILE((Dumper*)&list->dumper, #error);\*/\
    } while(0) \

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getLexemAnalysatorErrorMessage)\

#define DUMPER_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getDumperErrorMessage, DUMPER_STATUS_OK, LEXEM_ANALYSATOR_DUMPER_ERROR);

#define ARIFM_OPS_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getArifmOperationsErrorMessage, ARIFM_OPERATIONS_STATUS_OK, LEXEM_ANALYSATOR_ARIFM_OPS_ERROR);



const size_t MAX_NUM_OF_LEXEMS  = 1 << 9;
const size_t MAX_INPUT_LINE_LEN = 1 << 11;
const char* DELIMS    = " \n\t";
char*       tmpString = NULL;



LexemAnalysatorErrors constructLexemAnalysator(
    const char*      sourceFilePath,
    const char*      destFilePath,
    LexemAnalysator* analysator
) {
    IF_ARG_NULL_RETURN(sourceFilePath);
    IF_ARG_NULL_RETURN(destFilePath);
    IF_ARG_NULL_RETURN(analysator);

    *analysator = {
        .sourceFilePath = sourceFilePath,
        .destFilePath   = destFilePath,
        .arrLen         = (size_t)0,
        .array          = NULL,
        .inputString    = NULL,
        .inputStringLen = 0,
    };

    return LEXEM_ANALYSATOR_STATUS_OK;
}



//
//
// static bool isDelimSkip(const char** ptr) {
//     assert(ptr != NULL);
//     assert(*ptr != NULL);
//
//     if (strchr(DELIMS, **ptr) != NULL) {
//         ++(*ptr);
//         return true;
//     }
//
//     return false;
// }
//
// static bool tryNumber(LexemAnalysator* analysator, const char** ptr) {
//     assert(analysator != NULL);
//     assert(ptr != NULL);
//     assert(*ptr != NULL);
//
//     if (!isdigit(**ptr)) {
//         return false;
//     }
//
//     errno = 0;
//     char* endPtr = (char*)*ptr; // WARNING:
//     double num = strtod(*ptr, &endPtr);
//     if (errno != 0) {
//         return false;
//     }
//
//     LOG_DEBUG_VARS(num, *ptr, endPtr);
//     assert(analysator->arrLen < MAX_NUM_OF_LEXEMS);
//     analysator->array[analysator->arrLen++] = {
//         .nodeType = ARIFM_TREE_NUMBER_NODE,
//         {.doubleData = num},
//         .left = 0, .right = 0,
//         .memBuffIndex = 0,
//         .parent = 0,
//     };
//     *ptr = endPtr;
//     return true;
// }
//
// static bool tryBracket(LexemAnalysator* analysator, const char** ptr) {
//     assert(analysator != NULL);
//     assert(ptr != NULL);
//     assert(*ptr != NULL);
//
//     char ch = **ptr;
//     if (ch != '(' && ch != ')')
//         return false;
//
//     assert(analysator->arrLen < MAX_NUM_OF_LEXEMS);
//     LOG_DEBUG_VARS("brackets");
//     analysator->array[analysator->arrLen++] = {
//         .nodeType = ARIFM_TREE_FUNC_NODE,
//         {.data = (size_t)ch}, // ASK: is this ok
//         .left = 0, .right = 0,
//         .memBuffIndex = 0,
//         .parent = 0,
//     };
//     ++(*ptr);
//
//     return true;
// }
//
void clearTmpString() {
    size_t strLen = strlen(tmpString);
    while (strLen > 0) {
        tmpString[--strLen] = '\0';
    }
}

// static bool tryReadFuncName(LexemAnalysator* analysator, const char* ptr) {
//     assert(tmpString != NULL);
//     assert(analysator != NULL);
//
//     size_t strLen = strlen(tmpString);
//     if (strLen == 1 && isalpha(*(ptr - 1)) && !isalpha(*ptr)) { // this is variable
//         assert(analysator->arrLen < MAX_NUM_OF_LEXEMS);
//         analysator->array[analysator->arrLen++] = {
//             .nodeType = ARIFM_TREE_VAR_NODE,
//             {.data = (size_t)(*(ptr - 1) - 'a')}, // ASK: is this ok
//             .left = 0, .right = 0,
//             .memBuffIndex = 0,
//             .parent = 0,
//         };
//
//         clearTmpString();
//         return true;
//     }
//
//     // string is too long, so we asume
//     // FIXME: add err check
//     Function func = {};
//     ArifmOperationsErrors error = getFunctionByName(tmpString, &func);
//     if (error == ARIFM_OPERATIONS_FUNC_NOT_FOUND) {
//         return false;
//     }
//
//     LOG_ERROR("----------");
//     LOG_DEBUG_VARS("func name: ", tmpString);
//     assert(analysator->arrLen < MAX_NUM_OF_LEXEMS);
//     analysator->array[analysator->arrLen++] = {
//         .type =
//     };
//
//     clearTmpString();
//
//     return true;
// }

// LexemAnalysatorErrors getArrayOfLexems(LexemAnalysator* analysator) {
//     IF_ARG_NULL_RETURN(analysator);
//
//     analysator->arrLen = 0;
//     tmpString = (char*)calloc(MAX_INPUT_LINE_LEN, sizeof(char));
//     IF_NOT_COND_RETURN(tmpString != NULL,
//                        LEXEM_ANALYSATOR_MEMORY_ALLOCATION_ERROR);
//
//     const char* ptr = analysator->inputString;
//     while (*ptr != '\0') {
//         LOG_ERROR("----------");
//         if (isDelimSkip(            &ptr))      continue;
//         if (tryNumber  (analysator, &ptr))      continue;
//         if (tryBracket (analysator, &ptr))      continue;
//
//         size_t tmpStringLen = strlen(tmpString);
//         assert(tmpStringLen < MAX_INPUT_LINE_LEN);
//         tmpString[tmpStringLen] = *ptr;
//         ++ptr;
//         if (tryReadFuncName(analysator, ptr)) continue;
//     }
//
//     (analysator->array)[analysator->arrLen] =  { // terminal element, just like \0 is in string
//         .type = INVALID_LEXEM_TYPE,
//         .strRepr = NULL,
//         {.data = 0},
//     };
//
//     return LEXEM_ANALYSATOR_STATUS_OK;
// }

LexemAnalysatorErrors getArrayOfLexems(LexemAnalysator* analysator) {
    IF_ARG_NULL_RETURN(analysator);

    analysator->array = (Lexem*)calloc(MAX_NUM_OF_LEXEMS, sizeof(Lexem));
    IF_NOT_COND_RETURN(analysator->array != NULL,
                       LEXEM_ANALYSATOR_MEMORY_ALLOCATION_ERROR);

    //strcat(analysator->inputString, "\n"); // adding delim to the end of line

    tmpString = (char*)calloc(MAX_INPUT_LINE_LEN, sizeof(char));
    IF_NOT_COND_RETURN(tmpString != NULL, LEXEM_ANALYSATOR_MEMORY_ALLOCATION_ERROR);

    #define TRY2ADD_LEXEM2ARR() \
        do {                \
            if (strlen(tmpString) > 0) {\
                Lexem* lexem = &analysator->array[analysator->arrLen++];\
                initLexemWithString(tmpString, lexem); /* TODO: add error check */\
                clearTmpString();\
            }\
        } while (0)

    const char* naturalDelims = "\t\n ";
    size_t curCharInd = 0;
    bool isPrevLexemDelim = false;
    for (size_t i = 0; i < analysator->inputStringLen; ++i) {
        char curCh = i < analysator->inputStringLen ? analysator->inputString[i] : ' '; // ' ' is delimeter
        bool isDelim = false;
        isCharLexemDelim(curCh, &isDelim);

        bool isNaturalDelim = strchr(naturalDelims, curCh) != NULL;
        //LOG_DEBUG_VARS(curCh, i, tmpString, isDelim, isNaturalDelim);
        if (isDelim || isNaturalDelim || isPrevLexemDelim) {
            TRY2ADD_LEXEM2ARR();
            curCharInd = 0;
            if (isNaturalDelim) {
                isPrevLexemDelim = false;
                continue;
            }
        }

        tmpString[curCharInd++] = curCh;
        isPrevLexemDelim = false;
        if (isDelim)
            isPrevLexemDelim = true;
    }
    TRY2ADD_LEXEM2ARR();

    IF_ERR_RETURN(dumpLexemAnalysator(analysator));
    FREE(tmpString);

    return LEXEM_ANALYSATOR_STATUS_OK;
}

size_t getFileSize(FILE* file) {
    assert(file != NULL);

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    return fileSize;
}

// it will be easier to draw dump and parse this code
LexemAnalysatorErrors wrapAllCodeInBrackets(char** inputLine) {
    IF_ARG_NULL_RETURN(*inputLine);
    char* strCopy = (char*)calloc(strlen(*inputLine) + 1 + 2, sizeof(char));
    IF_NOT_COND_RETURN(strCopy != NULL, LEXEM_ANALYSATOR_MEMORY_ALLOCATION_ERROR);
    strcpy(strCopy, *inputLine);

    sprintf(*inputLine, "{\n%s\n}\n", strCopy);
    FREE(strCopy);
    return LEXEM_ANALYSATOR_STATUS_OK;
}

LexemAnalysatorErrors processSourceFile(LexemAnalysator* analysator) {
    IF_ARG_NULL_RETURN(analysator);

    FILE* file = fopen(analysator->sourceFilePath, "r");
    IF_NOT_COND_RETURN(file != NULL, LEXEM_ANALYSATOR_FILE_OPENING_ERROR);

    size_t fileSize = getFileSize(file);
    analysator->inputStringLen = fileSize;
    analysator->inputString = (char*)calloc(fileSize + 1 + 6, sizeof(char)); // +1 for \0 symbol, +4 for brackets and \n on both ends
    IF_NOT_COND_RETURN(analysator->inputString != NULL,
                       LEXEM_ANALYSATOR_MEMORY_ALLOCATION_ERROR);

    char* inputStringPtr = analysator->inputString;
    char lineBuffer[MAX_INPUT_LINE_LEN] = {};
    while (fgets(lineBuffer, MAX_INPUT_LINE_LEN, file)) {
        size_t leftSpace = fileSize - (inputStringPtr - analysator->inputString);
        strncat(inputStringPtr, lineBuffer, leftSpace);
        // inputStringPtr += snprintf("%s%s", fileSize - (inputStringPtr - analysator->inputString),
        //                            inputStringPtr, lineBuffer);
    }

    wrapAllCodeInBrackets(&analysator->inputString);
    analysator->inputStringLen = strlen(analysator->inputString);

    fclose(file);

    return LEXEM_ANALYSATOR_STATUS_OK;
}

LexemAnalysatorErrors saveArrayOfLexems2File(LexemAnalysator* analysator) {
    IF_ARG_NULL_RETURN(analysator);

    FILE* file = fopen(analysator->destFilePath, "w");
    IF_NOT_COND_RETURN(file != NULL, LEXEM_ANALYSATOR_FILE_OPENING_ERROR);

    for (size_t i = 0; i < analysator->arrLen; ++i) {
        Lexem lexem = analysator->array[i];
        size_t lexemType = lexem.type;
        size_t specName = lexem.lexemSpecificName;
        double doubleData = lexem.doubleData;

        saveLexemToFile(file, &lexem);
        fprintf(file, "\n");
    }

    fclose(file);

    return LEXEM_ANALYSATOR_STATUS_OK;
}

LexemAnalysatorErrors dumpLexemAnalysator(LexemAnalysator* analysator) {
    IF_ARG_NULL_RETURN(analysator);

    LOG_DEBUG("--------------------------");
    LOG_DEBUG("lexem analysator:");
    LOG_DEBUG_VARS(analysator->sourceFilePath);
    LOG_DEBUG_VARS(analysator->destFilePath);
    LOG_DEBUG_VARS(analysator->arrLen);
    LOG_DEBUG_VARS(analysator->inputString);
    LOG_DEBUG_VARS(analysator->inputStringLen);
    LOG_DEBUG("array: ");

    for (size_t i = 0; i < analysator->arrLen; ++i) {
        Lexem lexem = analysator->array[i];
        char* line = NULL;
        getLexemDebugString(&lexem, &line);
        const char* lexemType = getLexemTypeString(lexem.type);
        LOG_DEBUG_VARS(i, lexemType, line);
        FREE(line);
    }
    LOG_DEBUG_VARS("--------------------------");

    return LEXEM_ANALYSATOR_STATUS_OK;
}

LexemAnalysatorErrors openCurImageOfLexemAnalysator(LexemAnalysator* analysator);

LexemAnalysatorErrors destructLexemAnalysator(LexemAnalysator* analysator) {
    IF_ARG_NULL_RETURN(analysator);

    FREE(analysator->array);
    FREE(analysator->inputString);
    analysator->arrLen = 0;
    analysator->inputStringLen = 0;

    return LEXEM_ANALYSATOR_STATUS_OK;
}

