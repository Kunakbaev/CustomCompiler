#include <stdlib.h>

#include "../include/intermidRepr.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, INTERMID_REPR_INVALID_ARGUMENT, getIntermidReprErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getIntermidReprErrorMessage, INTERMID_REPR_STATUS_OK);

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getIntermidReprErrorMessage)

#define SEMANTIC_CHECKER_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getSemanticCheckerErrorMessage, SEMANTIC_CHECKER_STATUS_OK, INTERMID_REPR_SEMANTIC_CHECKER_ERROR)

#define SYNTAX_TREE_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getSyntaxTreeErrorMessage, SYNTAX_TREE_STATUS_OK, INTERMID_REPR_SYNTAX_TREE_ERROR)




#define INTERMID_REPR_CMD_DEF(strRepresentation, enumName, id) \
    {.commandType = enumName, .strRepr=strRepresentation},

static CommandTypeAndStrReprPair commandsStrReprs[] = {
    #include "../include/intermidReprCommandsCodeGen.inc"
};

#undef INTERMID_REPR_CMD_DEF



IntermidReprErrors constructIntermidReprFromSyntaxTree(
    const char*       sourceFilePath,
    Dumper*           dumper,
    IntermidRepr*     intermidRepr
) {
    IF_ARG_NULL_RETURN(sourceFilePath);
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(intermidRepr);

    *intermidRepr = {
        .sourceFilePath   = sourceFilePath,
        .funcReprListTail = NULL,
        .checker          = {},
    };

    SYNTAX_TREE_ERR_CHECK(constructSyntaxTree(&intermidRepr->tree, dumper));
    SEMANTIC_CHECKER_ERR_CHECK(constructSemanticChecker(&intermidRepr->checker, &intermidRepr->tree));

    return INTERMID_REPR_STATUS_OK;
}

IntermidReprErrors readIntermidReprSyntaxTreeFromFile(
    IntermidRepr* intermidRepr
) {
    IF_ARG_NULL_RETURN(intermidRepr);

    SYNTAX_TREE_ERR_CHECK(readSyntaxTreeFromFile(&intermidRepr->tree, intermidRepr->sourceFilePath));
    SYNTAX_TREE_ERR_CHECK(dumpSyntaxTree(&intermidRepr->tree));

    SEMANTIC_CHECKER_ERR_CHECK(buildTableOfIdentificators(&intermidRepr->checker));
    SEMANTIC_CHECKER_ERR_CHECK(dumpTableOfIdentificators(&intermidRepr->checker));
    SEMANTIC_CHECKER_ERR_CHECK(semanticCheckOfSyntaxTree(&intermidRepr->checker));
    SEMANTIC_CHECKER_ERR_CHECK(recursiveFindTinTout(&intermidRepr->checker, intermidRepr->checker.tree->root));

    return INTERMID_REPR_STATUS_OK;
}

static IntermidReprErrors addNewElementToFuncRepr(
    FunctionRepr*               funcRepr
) {
    IF_ARG_NULL_RETURN(funcRepr);

    IntermidReprElement* element =
        (IntermidReprElement*)calloc(1, sizeof(IntermidReprElement));
    IF_NOT_COND_RETURN(element != NULL, INTERMID_REPR_MEMORY_ALLOCATION_ERROR);

    if (funcRepr->listTail != NULL) {
        element->prevElement = funcRepr->listTail;
        funcRepr->listTail->nextElement = element;
    } else {
        funcRepr->listHead = element;
    }
    funcRepr->listTail = element;
    ++funcRepr->numOfElements;

    return INTERMID_REPR_STATUS_OK;
}

static IntermidReprErrors addNewCommandToFuncRepr(
    FunctionRepr*               funcRepr,
    IntermidReprCommandType     commandType,
    IntermidReprCommandArgument argument,
    size_t                      depthInBlocksOfCode
) {
    IF_ARG_NULL_RETURN(funcRepr);

    IF_ERR_RETURN(addNewElementToFuncRepr(funcRepr));

    IntermidReprElement* element = funcRepr->listTail;
    assert(element != NULL);
    element->depthInBlocksOfCode = depthInBlocksOfCode;
    element->isCommand           = true;
    element->command.argument    = argument;
    element->command.commandType = commandType;

    return INTERMID_REPR_STATUS_OK;
}

size_t numOfLabelsBefore = 0; // TODO: remove global variable

static IntermidReprErrors addNewLabelToFuncRepr(
    FunctionRepr* funcRepr,
    size_t        depthInBlocksOfCode
) {
    IF_ARG_NULL_RETURN(funcRepr);

    IF_ERR_RETURN(addNewElementToFuncRepr(funcRepr));

    IntermidReprElement* element = funcRepr->listTail;
    assert(element != NULL);
    element->depthInBlocksOfCode = depthInBlocksOfCode;
    element->isCommand = false;
    element->label.labelInd = numOfLabelsBefore++;

    return INTERMID_REPR_STATUS_OK;
}

static size_t getLocalVariableIndex(
    const FunctionRepr* function,
    char*               variableName
) {
    IF_ARG_NULL_RETURN(function);
    IF_ARG_NULL_RETURN(variableName);

    size_t ind = 0;
    const LocalVarsListNode* node = function->localVarsListTail;
    LOG_DEBUG_VARS(variableName);
    while (node != NULL) {
        int cmpRes = strcmp(node->name, variableName);
        LOG_DEBUG_VARS(node->name, variableName);
        if (cmpRes == 0) {
            // because we traverse elements in reverse
            return function->numOfLocalVars - ind - 1;
        }

        ++ind;
        node = node->prev;
    }

    assert(false && "variable is not found");
    return -1;
}

static IntermidReprErrors countNumOfFuncArgs(
    const IntermidRepr* intermidRepr,
    FunctionRepr*       function,
    size_t              curNodeInd
) {
    IF_ARG_NULL_RETURN(intermidRepr);
    IF_ARG_NULL_RETURN(function);

    if (!curNodeInd)
        return INTERMID_REPR_STATUS_OK;

    Node* node  = getSyntaxTreeNodePtr(&intermidRepr->tree, curNodeInd);
    Lexem lexem = node->lexem;

    if (lexem.type == IDENTIFICATOR_LEXEM_TYPE) {
        ++function->numOfArguments;
    }

    IF_ERR_RETURN(countNumOfFuncArgs(intermidRepr, function, node->left));
    IF_ERR_RETURN(countNumOfFuncArgs(intermidRepr, function, node->right));
}

static IntermidReprErrors countMaxNumberOfLocalVars(
    const IntermidRepr* intermidRepr,
    FunctionRepr*       function,
    size_t              curNodeInd,
    int*                result,          // number of local variables
    bool                isDeclaration
) {
    IF_ARG_NULL_RETURN(intermidRepr);
    IF_ARG_NULL_RETURN(function);
    IF_ARG_NULL_RETURN(result);

    if (!curNodeInd)
        return INTERMID_REPR_STATUS_OK;

    Node* node  = getSyntaxTreeNodePtr(&intermidRepr->tree, curNodeInd);
    Lexem lexem = node->lexem;

    LOG_DEBUG_VARS(curNodeInd, lexem.strRepr, isDeclaration);
    if (isDeclaration && lexem.type == IDENTIFICATOR_LEXEM_TYPE) {
        Identificator id = {};
        SEMANTIC_CHECKER_ERR_CHECK(getIdentificatorByLexem(&intermidRepr->checker, &lexem, &id));
        LOG_DEBUG_VARS(id.type, FUNCTION_IDENTIFICATOR, VARIABLE_IDENTIFICATOR);
        if (id.declNodeInd == curNodeInd &&
            id.type        == VARIABLE_IDENTIFICATOR) { // we found variable declaration 
            *result = *result + 1; // increase number of local variables

            LOG_DEBUG_VARS(id.declNodeInd, lexem.strRepr);
            LocalVarsListNode* newNode = (LocalVarsListNode*)calloc(1,                         sizeof(LocalVarsListNode));
            newNode->name              =              (char*)calloc(strlen(lexem.strRepr) + 1, sizeof(char));
            IF_NOT_COND_RETURN(newNode       != NULL, INTERMID_REPR_MEMORY_ALLOCATION_ERROR);
            IF_NOT_COND_RETURN(newNode->name != NULL, INTERMID_REPR_MEMORY_ALLOCATION_ERROR);
            strcpy(newNode->name, lexem.strRepr);

            newNode->prev = function->localVarsListTail;
            function->localVarsListTail = newNode;
        }
    }

    isDeclaration |= lexem.lexemSpecificName == KEYWORD_INT_LEXEM;
    IF_ERR_RETURN(countMaxNumberOfLocalVars(intermidRepr, function, node->left,  result, isDeclaration));
    IF_ERR_RETURN(countMaxNumberOfLocalVars(intermidRepr, function, node->right, result, isDeclaration));
    
    return INTERMID_REPR_STATUS_OK;
}

#include "delimsReprDefines.cpp"
#include "keywordsReprDefines.cpp"
#include "operatorsReprDefines.cpp"

static IntermidReprErrors addFunctionCallArguments(
    const IntermidRepr* intermidRepr,
    FunctionRepr*       function,
    size_t              curNodeInd,
    size_t              depthInBlocksOfCode
) {
    IF_ARG_NULL_RETURN(intermidRepr);
    IF_ARG_NULL_RETURN(function);

    if (!curNodeInd)
        return INTERMID_REPR_STATUS_OK;

    Node* node  = getSyntaxTreeNodePtr(&intermidRepr->tree, curNodeInd);
    Lexem lexem = node->lexem;

    if (lexem.type == IDENTIFICATOR_LEXEM_TYPE) {
        Identificator id = {};
        getIdentificatorByLexem(&intermidRepr->checker, &lexem, &id);
        if (id.type == VARIABLE_IDENTIFICATOR) {
            LOG_DEBUG_VARS(lexem.strRepr);
            size_t varInd = getLocalVariableIndex(function, lexem.strRepr);
            ADD_COMMAND(PARAM_IN_VAR_INTERMID_REPR_CMD, variableInd, varInd);
        }
    }
    if (lexem.type == CONST_LEXEM_TYPE) {
        ADD_COMMAND(PARAM_IN_NUM_INTERMID_REPR_CMD, doubleNumber, lexem.doubleData);
    }

    // pushing arguments in reverse order
    IF_ERR_RETURN(addFunctionCallArguments(intermidRepr, function, node->right, depthInBlocksOfCode));
    IF_ERR_RETURN(addFunctionCallArguments(intermidRepr, function, node->left,  depthInBlocksOfCode));

    return INTERMID_REPR_STATUS_OK;
}

static IntermidReprErrors parseFunctionRecursive(
    const IntermidRepr* intermidRepr,
    FunctionRepr*       function,
    size_t              curNodeInd,
    size_t              depthInBlocksOfCode
) {
    IF_ARG_NULL_RETURN(intermidRepr);
    IF_ARG_NULL_RETURN(function);

    if (!curNodeInd)
        return INTERMID_REPR_STATUS_OK;

    Node* node  = getSyntaxTreeNodePtr(&intermidRepr->tree, curNodeInd);
    Lexem lexem = node->lexem;

    LOG_DEBUG_VARS(lexem.strRepr, curNodeInd);
    bool isNum = lexem.type == CONST_LEXEM_TYPE;
    if (isNum || lexem.type == IDENTIFICATOR_LEXEM_TYPE) {
        if (isNum) {
            ADD_COMMAND(PUSH_NUM_INTERMID_REPR_CMD, doubleNumber, lexem.doubleData);
        } else {
            Identificator id = {};
            getIdentificatorByLexem(&intermidRepr->checker, &lexem, &id);
            if (id.type == VARIABLE_IDENTIFICATOR) {
                // node's parent is != NULL as we always have our code wrapped in brackets 
                // so root always exists and it's equal to { delim
                Node* parent = getSyntaxTreeNodePtr(&intermidRepr->tree, node->parent);
                Lexem parentLexem = parent->lexem;
                if (parentLexem.type              == OPERATOR_LEXEM_TYPE ||
                    parentLexem.lexemSpecificName == KEYWORD_RETURN_LEXEM) {
                    LOG_DEBUG_VARS(lexem.strRepr);
                    size_t varInd = getLocalVariableIndex(function, lexem.strRepr);
                    ADD_COMMAND(PUSH_VAR_INTERMID_REPR_CMD, variableInd, varInd);
                }
            } else {
                IF_ERR_RETURN(addFunctionCallArguments(intermidRepr, function, curNodeInd, depthInBlocksOfCode));
                ADD_COMMAND(CALL_INTERMID_REPR_CMD, functionName, lexem.strRepr);
                return INTERMID_REPR_STATUS_OK;
            }
        }

        GEN4LEFT();
        GEN4RIGHT();
        return INTERMID_REPR_STATUS_OK;
    }

    #define GENERAL_LEXEM_DEF(_, specName, ...) \
        case specName:                          \
            specName##_REPR_DEF();              \
            break;                              \

    switch (lexem.lexemSpecificName) {
        #include "../../LexemsRealizations/include/codeGen/allLexems.hpp"
        default:
            LOG_DEBUG_VARS(lexem.lexemSpecificName, lexem.strRepr);
            assert(false);
    }

    #undef GENERAL_LEXEM_DEF

    return INTERMID_REPR_STATUS_OK;
}

static IntermidReprErrors passFuncArguments(
    const IntermidRepr* intermidRepr,
    FunctionRepr*       function,
    Lexem               lexem
) {
    size_t depthInBlocksOfCode = 1; // this variable is actually used in ADD_COMMAND define
    Identificator id = {};
    getIdentificatorByLexem(&intermidRepr->checker, &lexem, &id);
    // poping arguments in reverse order, so that first argument we pop is the last one
    for (ssize_t argInd = 0; argInd < function->numOfArguments; ++argInd) {
        size_t varArrInd = id.function.argumentVars[argInd];
        Identificator var = intermidRepr->checker.tableOfVars[varArrInd]; // TODO: add getter func
        //LOG_DEBUG_VARS(varArrInd, var.lexem.strRepr, var.lexem.type);
        ADD_COMMAND(PARAM_OUT_INTERMID_REPR_CMD, variableInd, argInd);
    }

    return INTERMID_REPR_STATUS_OK;
}

static IntermidReprErrors parseFunction(
    const IntermidRepr* intermidRepr,
    FunctionRepr*       function,
    size_t              curNodeInd
) {
    IF_ARG_NULL_RETURN(intermidRepr);
    IF_ARG_NULL_RETURN(function);

    if (!curNodeInd)
        return INTERMID_REPR_STATUS_OK;

    Node* node  = getSyntaxTreeNodePtr(&intermidRepr->tree, curNodeInd);
    Lexem lexem = node->lexem;

    function->numOfLocalVars = 0;
    LOG_ERROR(function->functionName);
    IF_ERR_RETURN(countNumOfFuncArgs(intermidRepr, function, node->left));
    IF_ERR_RETURN(countMaxNumberOfLocalVars(intermidRepr, function, curNodeInd, &function->numOfLocalVars, false));
    function->numOfElements = 0;

    size_t depthInBlocksOfCode = 1; // not a very good solution, but defines for func call works this way
    ADD_COMMAND(ENTER_INTERMID_REPR_CMD, intNumber, function->numOfLocalVars);
    depthInBlocksOfCode = 0;

    IF_ERR_RETURN(passFuncArguments(intermidRepr, function, lexem));

    numOfLabelsBefore = 0;
    // we need to traverse only right subtree, as left one contains
    // only function's arguments declaration
    GEN4RIGHT();

    return INTERMID_REPR_STATUS_OK;
}

static IntermidReprErrors addNewFunctionToRepr(
    IntermidRepr*       intermidRepr,
    size_t              funcDeclNodeInd
) {
    IF_ARG_NULL_RETURN(intermidRepr);

    Node* node  = getSyntaxTreeNodePtr(&intermidRepr->tree, funcDeclNodeInd);
    Lexem lexem = node->lexem;

    FunctionRepr* funcRepr = (FunctionRepr*)calloc(1, sizeof(FunctionRepr));
    IF_NOT_COND_RETURN(funcRepr != NULL, INTERMID_REPR_MEMORY_ALLOCATION_ERROR);

    // set function name = copy strRepr from lexem
    size_t funcNameLen = strlen(lexem.strRepr);
    funcRepr->functionName = (char*)calloc(funcNameLen + 1, sizeof(char));
    IF_NOT_COND_RETURN(funcRepr->functionName != NULL,
                       INTERMID_REPR_MEMORY_ALLOCATION_ERROR);
    strcpy(funcRepr->functionName, lexem.strRepr);

    IF_ERR_RETURN(parseFunction(intermidRepr, funcRepr, funcDeclNodeInd));

    if (intermidRepr->funcReprListTail != NULL) {
        funcRepr->prevFunc = intermidRepr->funcReprListTail;
        intermidRepr->funcReprListTail->nextFunc = funcRepr;
    } else {
        intermidRepr->funcReprListHead = funcRepr;
    }
    intermidRepr->funcReprListTail = funcRepr;

    return INTERMID_REPR_STATUS_OK;
}

static IntermidReprErrors findAndProcessFuncs(
    size_t            curNodeInd,
    IntermidRepr*     intermidRepr
) {
    IF_ARG_NULL_RETURN(intermidRepr);

    if (!curNodeInd)
        return INTERMID_REPR_STATUS_OK;

    Node* node  = getSyntaxTreeNodePtr(&intermidRepr->tree, curNodeInd);
    Lexem lexem = node->lexem;

    IF_NOT_COND_RETURN(lexem.type != INVALID_LEXEM_TYPE,
                       INTERMID_REPR_INVALID_ARGUMENT);

    if (lexem.type == IDENTIFICATOR_LEXEM_TYPE) {
        Identificator id = {};
        getIdentificatorByLexem(&intermidRepr->checker, &lexem, &id);
        if (curNodeInd == id.declNodeInd &&
            id.type    == FUNCTION_IDENTIFICATOR) { // we found function declaration
            IF_ERR_RETURN(addNewFunctionToRepr(intermidRepr, curNodeInd));
        }
    }

    IF_ERR_RETURN(findAndProcessFuncs(node->left,  intermidRepr));
    IF_ERR_RETURN(findAndProcessFuncs(node->right, intermidRepr));

    return INTERMID_REPR_STATUS_OK;
}

IntermidReprErrors getIntermidRepresentation(
    IntermidRepr* intermidRepr
) {
    IF_ARG_NULL_RETURN(intermidRepr);

    IF_ERR_RETURN(findAndProcessFuncs(intermidRepr->tree.root, intermidRepr));

    return INTERMID_REPR_STATUS_OK;
}

IntermidReprErrors getCommandStringRepr(
    IntermidReprCommandType commandType,
    const char**            result
) {
    IF_ARG_NULL_RETURN(result);

    const size_t NUM_OF_COMMANDS = sizeof(commandsStrReprs) / sizeof(*commandsStrReprs);
    for (size_t i = 0; i < NUM_OF_COMMANDS; ++i) {
        if (commandType == commandsStrReprs[i].commandType) {
            *result = commandsStrReprs[i].strRepr;
            return INTERMID_REPR_STATUS_OK;
        }
    }

    return INTERMID_REPR_COMMAND_TYPE_NOT_FOUND;
}

// WARNING: this function uses buffer which is a static variable
char* getLabelNameById(
    const FunctionRepr* function,
    size_t              labelInd
) {
    assert(function != NULL);

    const size_t MAX_LABEL_NAME_LEN             = 100;
    static char  buffer[MAX_LABEL_NAME_LEN + 1] = {};
    snprintf(buffer, MAX_LABEL_NAME_LEN, "%s_L%d", function->functionName, labelInd);

    return buffer;
}

static IntermidReprErrors addStringRepresentation2File(
    const FunctionRepr*        function,
    const IntermidReprCommand* command,
    FILE*                      file
) {
    IF_ARG_NULL_RETURN(function);
    IF_ARG_NULL_RETURN(command);
    IF_ARG_NULL_RETURN(file);

    const char* strRepr = NULL;
    IntermidReprCommandType cmdType = command->commandType;
    IF_ERR_RETURN(getCommandStringRepr(cmdType, &strRepr));
    fprintf(file, "%s", strRepr);

    switch (cmdType) {
        case PARAM_IN_NUM_INTERMID_REPR_CMD:
        case PUSH_NUM_INTERMID_REPR_CMD:
            fprintf(file, " %.3f\n", command->argument.doubleNumber);
            break;
        case PARAM_IN_VAR_INTERMID_REPR_CMD:
        case PUSH_VAR_INTERMID_REPR_CMD:
            fprintf(file, " @%zu\n", command->argument.variableInd);
            break;
        case PARAM_OUT_INTERMID_REPR_CMD:
        case POP_VAR_INTERMID_REPR_CMD:
            fprintf(file, " @%zu\n", command->argument.variableInd);
            break;
        case UNCOND_JMP_INTERMID_REPR_CMD:
        case IF_EQ_JMP_INTERMID_REPR_CMD:
            fprintf(file, " %s\n", getLabelNameById(function, command->argument.jumpDestLabelInd));
            break;
        case CALL_INTERMID_REPR_CMD:
            fprintf(file, " %s\n", command->argument.functionName);
            break;
        case ENTER_INTERMID_REPR_CMD:
            fprintf(file, " %d\n", command->argument.intNumber);
            break;
        default:
            fprintf(file, "\n");
            break;
    }

    return INTERMID_REPR_STATUS_OK;
}

IntermidReprErrors dumpTextVersionOfIntermidRepr2file(
    IntermidRepr* intermidRepr,
    const char*   destFilePath
) {
    IF_ARG_NULL_RETURN(intermidRepr);
    IF_ARG_NULL_RETURN(destFilePath);

    FILE* file = fopen(destFilePath, "w");

    const size_t MAX_DEPTH_IN_BLOCKS_OF_CODE             = 50;
    char         tabsBuffer[MAX_DEPTH_IN_BLOCKS_OF_CODE] = {};

    FunctionRepr* funcPtr = intermidRepr->funcReprListHead;
    while (funcPtr != NULL) {
        fprintf(file, "%s(", funcPtr->functionName);
        for (int i = 0; i < funcPtr->numOfArguments; ++i) {
            if (i) fprintf(file, ", ");
            fprintf(file, "int");
        }
        fprintf(file, "):\n");

        IntermidReprElement* elemPtr = funcPtr->listHead;
        while (elemPtr != NULL) {
            // clear tabs buffer
            memset(tabsBuffer, 0, MAX_DEPTH_IN_BLOCKS_OF_CODE * sizeof(char));
            for (size_t i = 0; i < elemPtr->depthInBlocksOfCode; ++i)
                tabsBuffer[i] = '\t';
            // add tabs to indent blocks of code
            fprintf(file, "%s", tabsBuffer);
            LOG_DEBUG_VARS(elemPtr->depthInBlocksOfCode);

            if (elemPtr->isCommand) {
                IF_ERR_RETURN(addStringRepresentation2File(funcPtr, &elemPtr->command, file));
            } else {
                fprintf(file, "%s:\n", getLabelNameById(funcPtr, elemPtr->label.labelInd));
            }
            elemPtr = elemPtr->nextElement;
        }
        fprintf(file, "\n"); // so it's easier to see where one function ends and another starts
        
        funcPtr = funcPtr->nextFunc;
    }

    fclose(file);

    return INTERMID_REPR_STATUS_OK;
}

IntermidReprErrors dumpArrayVersionOfIntermidRepr(
    IntermidRepr* intermidRepr
) {
    IF_ARG_NULL_RETURN(intermidRepr);



    return INTERMID_REPR_STATUS_OK;
}

static IntermidReprErrors destructFunctionRepr(
    FunctionRepr* function
) {
    IF_ARG_NULL_RETURN(function);

    LocalVarsListNode* varNodePtr = function->localVarsListTail;
    while (varNodePtr != NULL) {
        LocalVarsListNode* preVarNode = varNodePtr->prev;
        free(varNodePtr->name);
        free(varNodePtr);
        varNodePtr = preVarNode;
    }

    IntermidReprElement* elemPtr = function->listTail;
    while (elemPtr != NULL) {
        IntermidReprElement* preElem = elemPtr->prevElement;
        free(elemPtr);
        elemPtr = preElem;
    }

    free(function->functionName);
    free(function);

    return INTERMID_REPR_STATUS_OK;
}

IntermidReprErrors destructIntermidRepr(
    IntermidRepr* intermidRepr 
) {
    IF_ARG_NULL_RETURN(intermidRepr);

    FunctionRepr* funcPtr = intermidRepr->funcReprListTail;
    while (funcPtr != NULL) {
        FunctionRepr* preFunc = funcPtr->prevFunc;
        IF_ERR_RETURN(destructFunctionRepr(funcPtr));
        funcPtr = preFunc;
    }

    SYNTAX_TREE_ERR_CHECK(destructSyntaxTree(&intermidRepr->tree));
    SEMANTIC_CHECKER_ERR_CHECK(destructSemanticChecker(&intermidRepr->checker));

    return INTERMID_REPR_STATUS_OK;
}
