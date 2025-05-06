#include "../include/commonFileStart.hpp"
#include "../../Dumper/include/dumper.hpp"

const size_t OUTPUT_BUFFER_SIZE = 1 << 18;

static SyntaxTreeErrors dumpSyntaxTreeInConsoleRecursive(
    const SyntaxTree*           tree,
    size_t                      nodeIndex,
    char**                      outputBuffer,
    const Lexem2stringSettings* settings
) {
    IF_ARG_NULL_RETURN(tree);
    IF_ARG_NULL_RETURN(outputBuffer);
    IF_ARG_NULL_RETURN(*outputBuffer);

    if (nodeIndex == 0) {
        strncat(*outputBuffer, "?", OUTPUT_BUFFER_SIZE);
        ++(*outputBuffer);
        return SYNTAX_TREE_STATUS_OK;
    }

    strncat(*outputBuffer, "(", OUTPUT_BUFFER_SIZE);
    ++(*outputBuffer);

    Node node = *getSyntaxTreeNodePtr(tree, nodeIndex);
    Lexem lexem = node.lexem;
    IF_ERR_RETURN(dumpSyntaxTreeInConsoleRecursive(tree, node.left, outputBuffer, settings));

    char* nodeDataString = NULL;
    LEXEMS_REALIZATIONS_ERR_CHECK(getLexemDataString(&lexem, settings, &nodeDataString));
    (*outputBuffer) += snprintf(*outputBuffer, OUTPUT_BUFFER_SIZE, " %s ", nodeDataString);
    FREE(nodeDataString);

    IF_ERR_RETURN(dumpSyntaxTreeInConsoleRecursive(tree, node.right, outputBuffer, settings));

    strncat(*outputBuffer, ")", OUTPUT_BUFFER_SIZE);
    ++(*outputBuffer);

    return SYNTAX_TREE_STATUS_OK;
}

SyntaxTreeErrors dumpSyntaxTreeInConsole(const SyntaxTree* tree) {
    IF_ARG_NULL_RETURN(tree);

    char* outputBuffer = (char*)calloc(OUTPUT_BUFFER_SIZE, sizeof(char));
    IF_NOT_COND_RETURN(outputBuffer != NULL,
                       SYNTAX_TREE_MEMORY_ALLOCATION_ERROR);

    Lexem2stringSettings lexemDumpSettings = {
        .isLexemTypeNeeded = true,
        .isBracketsNeeded  = true,
    };

    char* targetPtr = outputBuffer;
    IF_ERR_RETURN(dumpSyntaxTreeInConsoleRecursive(tree, tree->root, &targetPtr, &lexemDumpSettings));
    DEBUG_MESSAGE_TO_DUMPER_ALL_LOGS_FILE(tree->dumper, outputBuffer);
    FREE(outputBuffer);

    return SYNTAX_TREE_STATUS_OK;
}

SyntaxTreeErrors dumpSyntaxTree(SyntaxTree* tree) {
    IF_ARG_NULL_RETURN(tree);

    LOG_DEBUG("--------------------------------------\n");
    LOG_DEBUG("syntax tree:\n");
    DEBUG_MESSAGE_TO_DUMPER_ALL_LOGS_FILE(tree->dumper, "--------------------------------------\n");
    DEBUG_MESSAGE_TO_DUMPER_ALL_LOGS_FILE(tree->dumper, "syntax tree:\n");

    Lexem2stringSettings lexemDumpSettings = {
        .isLexemTypeNeeded = false,
        .isBracketsNeeded  = false,
    };

    const size_t NUM_OF_DIFF_LEXEM_TYPES = 6;
    size_t  diffNodeTypesPtr[NUM_OF_DIFF_LEXEM_TYPES + 1] = {};
    size_t* diffNodeTypes   [NUM_OF_DIFF_LEXEM_TYPES + 1] = {};
    for (size_t i = 1; i <= NUM_OF_DIFF_LEXEM_TYPES; ++i) {
        diffNodeTypes[i] = (size_t*)calloc(MAX_NUM_OF_NODES_IN_ONE_COLOR_WITH_NODES_STRUCT, sizeof(size_t));
        IF_NOT_COND_RETURN(diffNodeTypes[i] != NULL,
                           SYNTAX_TREE_MEMORY_ALLOCATION_ERROR);
    }
    for (size_t nodeInd = 1; nodeInd <= tree->freeNodeIndex; ++nodeInd) {
        Node node = *getSyntaxTreeNodePtr(tree, nodeInd);
        Lexem lexem = node.lexem;
        if (lexem.type == INVALID_LEXEM_TYPE)
            continue;
        char* data = NULL;
        LEXEMS_REALIZATIONS_ERR_CHECK(getLexemDataString(&lexem, &lexemDumpSettings, &data));

        size_t parent = node.parent;
        size_t left   = node.left;
        size_t right  = node.right;
        DEBUG_VARS_TO_DUMPER_ALL_LOGS_FILE(tree->dumper, nodeInd, data, parent, left, right);
        FREE(data);

        LexemType id = lexem.type;
        assert(1 <= id && id <= NUM_OF_DIFF_LEXEM_TYPES);
        assert(diffNodeTypesPtr[id] < MAX_NUM_OF_NODES_IN_ONE_COLOR_WITH_NODES_STRUCT);
        diffNodeTypes[id][diffNodeTypesPtr[id]++] = nodeInd;
    }

    IF_ERR_RETURN(dumpSyntaxTreeInConsole(tree));

    size_t nodesArr[MAX_NUM_OF_NODES_IN_ONE_COLOR_WITH_NODES_STRUCT] = {}; // ASK: is it ok?
    nodesArr[0] = {tree->freeNodeIndex};
    NodesWithColor rule1 = {
        .color       = "#004000",
        .borderColor = "#006000",
        .numOfNodes  = (size_t)1,
        .nodes       = nodesArr,
    };

    #define ARR_LEN(arr) sizeof((arr)) / sizeof(*(arr))
    #define NODE_RULE(name, colorVar, borderColorVar, ind)     \
        NodesWithColor rule4##name = {                         \
            .color       = colorVar,                           \
            .borderColor = borderColorVar,                     \
            .numOfNodes  = diffNodeTypesPtr[ind],              \
            .nodes       = diffNodeTypes[ind],                 \
        }                                                      \

    NODE_RULE(consts,          "#dd571c", "#ff571c", CONST_LEXEM_TYPE);
    NODE_RULE(identificators,  "#400040", "#600060", IDENTIFICATOR_LEXEM_TYPE);
    NODE_RULE(operators,       "#600000", "#800000", OPERATOR_LEXEM_TYPE);
    NODE_RULE(keywords,        "#0050ff", "#0030cf", KEYWORD_LEXEM_TYPE);
    NODE_RULE(delims,          "#006000", "#008000", DELIM_LEXEM_TYPE);

    NodesWithColor coloringRule[] = {
        rule1, rule4consts, rule4identificators, rule4operators, rule4keywords, rule4delims,
    };
    DumperSettings settings = {
        .coloringRule           = coloringRule,
        .coloringRuleLen        = ARR_LEN(coloringRule),
        .isMemIndexesInfoNeeded = false,
        .lexem2stringSettings   = lexemDumpSettings,
    };
    DUMPER_ERR_CHECK(dumperDumpSyntaxTree(tree->dumper, tree, &settings));
    for (int i = 1; i <= NUM_OF_DIFF_LEXEM_TYPES; ++i)
        FREE(diffNodeTypes[i]);

    return SYNTAX_TREE_STATUS_OK;
}

SyntaxTreeErrors openImageOfCurrentStateSyntaxTree(SyntaxTree* tree) {
    IF_ARG_NULL_RETURN(tree);
    //RETURN_IF_INVALID();
    //LOG_WARNING("--------------@@@@@@@@@@@@@@@@@@@@@");

    // first we need to create image of current state of tree
    IF_ERR_RETURN(dumpSyntaxTree(tree));

    // FIXME: be carefull with nasty commands
    const char* fileName = getLastImageFileName(tree->dumper);
    const size_t TMP_LEN = 100;
    char tmp[TMP_LEN]    = {};
    snprintf(tmp, TMP_LEN, "xdg-open %s", fileName);
    system(tmp);
    exit(0);

    return SYNTAX_TREE_STATUS_OK;
}
