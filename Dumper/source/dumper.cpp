#include <stdlib.h>

#include "../include/dumper.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, DUMPER_ERROR_INVALID_ARGUMENT, getDumperErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getDumperErrorMessage, DUMPER_STATUS_OK)

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getDumperErrorMessage)

#define ARIFM_TREE_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getSyntaxTreeErrorMessage, ARIFM_TREE_STATUS_OK, DUMPER_ERROR_ARIFM_TREE_ERROR);

// TODO: add error appropriate
#define LEXEMS_REALIZATION_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getLexemsRealizationsErrorMessage, LEXEMS_REALIZATIONS_STATUS_OK, DUMPER_ERROR_ARIFM_OPS_ERROR);

const size_t FILE_NAME_BUFFER_SIZE      = 100;
const size_t FULL_FILE_NAME_BUFFER_SIZE = 200;
const size_t BUFFER_SIZE                = 1 << 20;
const size_t TMP_BUFFER_SIZE            = 1 << 20;
const size_t FORMAT_SPEC_LEN            = 5;
const size_t COLOR_LEN                  = 10;

static char* fileFullNameBuffer         = NULL;
static char* fileNameBuffer             = NULL;
static char* tmpBuffer                  = NULL; // ASK: how to get rid of it? Like, make strcat and sprintf at the same time
static char* buffer                     = NULL;

const char* getDumperErrorMessage(DumperErrors error) {
    switch (error) {
        case DUMPER_STATUS_OK:
            return "No errors in linked list, everything is valid.\n";
        case DUMPER_ERROR_INVALID_ARGUMENT:
            return "Dumper error: invalid argument (possibly set to NULL).\n";
        case DUMPER_ERROR_MEMORY_ALLOCATION_ERROR:
            return "Dumper error: memory allocation error.\n";
        case DUMPER_ERROR_COULD_OPEN_FILE:
            return "Dumper error: couldn't open file.\n";
        case DUMPER_ERROR_ARIFM_OPS_ERROR:
            return "Dumper error: error occured in arifmetic operations sublib.\n";

        default:
            return "Dumper error: unknown error.\n";
    }
}

DumperErrors dumperConstructor(
    Dumper*     dumper,
    size_t      maxNumOfNodesToDraw,
    const char* dirForLogsPath,
    const char* outputFileFormat
) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(dirForLogsPath);
    IF_ARG_NULL_RETURN(outputFileFormat);

    *dumper = {};
    dumper->maxNumOfNodesToDraw = maxNumOfNodesToDraw;
    dumper->dirForLogsPath      = dirForLogsPath;
    dumper->outputFileFormat    = outputFileFormat;
    dumper->numberOfLogsBefore  = 0;

    fileFullNameBuffer = (char*)calloc(FULL_FILE_NAME_BUFFER_SIZE, sizeof(char));
    fileNameBuffer     = (char*)calloc(FILE_NAME_BUFFER_SIZE,      sizeof(char));
    tmpBuffer          = (char*)calloc(TMP_BUFFER_SIZE,            sizeof(char));
    buffer             = (char*)calloc(BUFFER_SIZE,                sizeof(char));
            
    IF_NOT_COND_RETURN(fileFullNameBuffer != NULL, DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);
    IF_NOT_COND_RETURN(fileNameBuffer     != NULL, DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);
    IF_NOT_COND_RETURN(tmpBuffer          != NULL, DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);
    IF_NOT_COND_RETURN(buffer             != NULL, DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);

    // WARNING: whole root directory can be deleted, bruh, or some nasty command executed
    // memset(fileFullNameBuffer, 0, FULL_FILE_NAME_BUFFER_SIZE);
    // snprintf(fileFullNameBuffer, FULL_FILE_NAME_BUFFER_SIZE, "rm -rf %s", dirForLogsPath);
    // system(fileFullNameBuffer);
    memset(fileFullNameBuffer, 0, FULL_FILE_NAME_BUFFER_SIZE);
    snprintf(fileFullNameBuffer, FULL_FILE_NAME_BUFFER_SIZE, "mkdir -p %s", dirForLogsPath);
    system(fileFullNameBuffer);
    system("mkdir -p logs/images");
    system("mkdir -p logs/html");
    system("mkdir -p logs/dots");

    char* allLogsFilePath = (char*)calloc(FULL_FILE_NAME_BUFFER_SIZE, sizeof(char));
    IF_NOT_COND_RETURN(allLogsFilePath != NULL,
                       DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);
    snprintf(allLogsFilePath, FULL_FILE_NAME_BUFFER_SIZE,
             "%s/allLogs.html", dumper->dirForLogsPath);
    dumper->allLogsFile = fopen(allLogsFilePath, "w");
    IF_NOT_COND_RETURN(dumper->allLogsFile != NULL,
                       DUMPER_ERROR_COULD_OPEN_FILE);
    FREE(allLogsFilePath);
    setvbuf(dumper->allLogsFile, NULL, 0, _IONBF);

    dumperAddDebugInfoToAllLogsFile(dumper, "<body style=\"background: black; overflow: scroll; margin: 15px\">\n");

    return DUMPER_STATUS_OK;
}

void dumperAddDebugInfoToAllLogsFile(const Dumper* dumper, const char* debugInfo) {
    assert(dumper    != NULL);
    assert(debugInfo != NULL);

    fprintf(dumper->allLogsFile, debugInfo);
}

void dumperAddImgToAllLogsFile(const Dumper* dumper, const char* imagePath) {
    assert(dumper    != NULL);
    assert(imagePath != NULL);

    fprintf(dumper->allLogsFile, "<img src=\"%s\"></img>\n", imagePath);
}

static DumperErrors addNodeDumpStructToBuffer(
    const Dumper*         dumper,
    const Node*           node,
    const char*           color,
    const char*           borderColor,
    const DumperSettings* settings
) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(settings);
    IF_ARG_NULL_RETURN(buffer);

    memset(tmpBuffer, 0, TMP_BUFFER_SIZE);
    if (node != NULL) {
        char* lexemDataStr = NULL;
        LEXEMS_REALIZATION_ERR_CHECK(getLexemDataString(&node->lexem, &settings->lexem2stringSettings, &lexemDataStr));
        #include "../../SyntaxAnalysator/include/syntaxAnalysator.hpp"
        Lexems id = node->lexem.lexemSpecificName;
        if (id == OPERATOR_LESS_LEXEM)             strcpy(lexemDataStr, "&lt;");
        if (id == OPERATOR_GREATER_LEXEM)          strcpy(lexemDataStr, "&gt;");
        if (id == OPERATOR_LESS_OR_EQUAL_LEXEM)    strcpy(lexemDataStr, "&lt;=");
        if (id == OPERATOR_GREATER_OR_EQUAL_LEXEM) strcpy(lexemDataStr, "&gt;=");

        char* tmpPtr = tmpBuffer;
        tmpPtr += snprintf(tmpPtr, TMP_BUFFER_SIZE - (tmpPtr - tmpBuffer),
        "iamnode_id_%zu [shape=circle, style=filled, fillcolor=\"%s\" margin=0, penwidth=\"3\" fontcolor=white, color=\"%s\", label=< \n"
            "<TABLE cellspacing=\"0\" border=\"0\"> \n"
                "<TR><TD colspan=\"2\">%s</TD></TR>\n",
                node->memBuffIndex, color, borderColor, lexemDataStr);

        if (settings->isMemIndexesInfoNeeded) {
            tmpPtr += snprintf(tmpPtr, TMP_BUFFER_SIZE - (tmpPtr - tmpBuffer),
                    "<TR><TD colspan=\"2\">memIndex:  %zu</TD></TR>\n"
                    "<TR><TD>left:  %zu</TD>\n"
                    "<TD>right: %zu</TD></TR>\n",
                    node->memBuffIndex, node->left, node->right);
        }

        tmpPtr += snprintf(tmpPtr, TMP_BUFFER_SIZE - (tmpPtr - tmpBuffer),"</TABLE> \n" " >];\n");

        FREE(lexemDataStr);
    } else {
        snprintf(tmpBuffer, TMP_BUFFER_SIZE,
            "iamnode_id_%zu [shape=rect, margin=0, fontcolor=white, color=%s, label=<null>];\n",
            node->memBuffIndex, color);
    }

    strncat(buffer, tmpBuffer, BUFFER_SIZE);

    return DUMPER_STATUS_OK;
}

DumperErrors dumperDumpSingleTreeNode(Dumper* dumper,
                                      const Node* node, const char* nodeColor, const char* borderColor,
                                      const DumperSettings* settings) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(settings);

    LOG_DEBUG("single node dumping ---------------------");
    ++dumper->numberOfLogsBefore;
    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE,
             "dots/%zu_node.dot", dumper->numberOfLogsBefore);

    memset(fileFullNameBuffer, 0, FULL_FILE_NAME_BUFFER_SIZE);
    snprintf(fileFullNameBuffer, FULL_FILE_NAME_BUFFER_SIZE,
             "%s/%s", dumper->dirForLogsPath, fileNameBuffer);

    FILE* outputFile = fopen(fileFullNameBuffer, "w");
    IF_NOT_COND_RETURN(outputFile != NULL,
                       DUMPER_ERROR_COULD_OPEN_FILE);

    memset(buffer, 0, BUFFER_SIZE);
    strncat(buffer, 
    "digraph html {\n\
        overlap=false\n\
        splines=ortho\n\
        bgcolor=\"black\"\n\
        rankdir=TB\n\
        pad=0.2\n\
    ", BUFFER_SIZE);

    IF_ERR_RETURN(addNodeDumpStructToBuffer(dumper, node, nodeColor, borderColor, settings));
    strncat(buffer, "}\n", BUFFER_SIZE);
    fprintf(outputFile, buffer);
    fclose(outputFile);

    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE, "%zu_node.%s",
            dumper->numberOfLogsBefore, dumper->outputFileFormat);

    memset(fileFullNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileFullNameBuffer, FULL_FILE_NAME_BUFFER_SIZE,
            "dot -Tpng logs/dots/%zu_node.dot -o  %s/images/%s",
            dumper->numberOfLogsBefore, dumper->dirForLogsPath, fileNameBuffer);
    system(fileFullNameBuffer);
    return DUMPER_STATUS_OK;
}







static const char* DEFAULT_COLOR = "black";

static const char* getNodeColor(
    const Node*           node,
    const DumperSettings* settings,
    const char**          borderColorRes
) {
    assert(node                      != NULL);
    assert(settings                  != NULL);
    assert(settings->coloringRule    != NULL);
    assert(settings->coloringRuleLen  < MAX_COLORING_RULE_LEN);
    assert(borderColorRes            != NULL);

    for (size_t arrInd = 0; arrInd < settings->coloringRuleLen; ++arrInd) {
        const char* color       = settings->coloringRule[arrInd].color;
        const char* borderColor = settings->coloringRule[arrInd].borderColor;
        size_t*     nodes       = settings->coloringRule[arrInd].nodes;
        size_t      nodesLen    = settings->coloringRule[arrInd].numOfNodes;

        for (size_t nodeArrInd = 0; nodeArrInd < nodesLen; ++nodeArrInd) {
            size_t nodeInd = nodes[nodeArrInd];
            if (nodeInd == node->memBuffIndex) {
                *borderColorRes = borderColor;
                return color;
            }
        }
    }

    return DEFAULT_COLOR; // maybe log error
}

static DumperErrors drawSyntaxTreeRecursively(
    const Dumper*         dumper,
    const SyntaxTree*     tree,
    size_t                nodeInd,
    size_t                parentInd,
    const DumperSettings* settings
) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(tree);
    IF_ARG_NULL_RETURN(settings);

    if (nodeInd == 0) // subtree is empty
        return DUMPER_STATUS_OK;

    Node node = *getSyntaxTreeNodePtr(tree, nodeInd);

    const char* borderColor = NULL;
    const char* color = getNodeColor(&node, settings, &borderColor);
    IF_ERR_RETURN(addNodeDumpStructToBuffer(dumper, &node, color, borderColor, settings));

    if (parentInd != 0) {
        memset(tmpBuffer, 0, TMP_BUFFER_SIZE);

        Node parent = *getSyntaxTreeNodePtr(tree, parentInd);
        if (nodeInd == parent.left) {
            snprintf(tmpBuffer, TMP_BUFFER_SIZE, "iamnode_id_%zu -> iamnode_id_%zu [color=orange, fontcolor=white, weight=1]\n",
                parentInd, nodeInd);
        } else {
            snprintf(tmpBuffer, TMP_BUFFER_SIZE, "iamnode_id_%zu -> iamnode_id_%zu [color=lightblue, fontcolor=white, weight=1]\n",
                parentInd, nodeInd);
        }

        size_t tmpBuffLen = strlen(tmpBuffer);
        snprintf(tmpBuffer + tmpBuffLen, TMP_BUFFER_SIZE - tmpBuffLen, "iamnode_id_%zu -> iamnode_id_%zu [color=purple, fontcolor=white]\n",
                nodeInd, parentInd);
        strncat(buffer, tmpBuffer, BUFFER_SIZE);
    }

    IF_ERR_RETURN(drawSyntaxTreeRecursively(dumper, tree, node.left,  nodeInd, settings));
    IF_ERR_RETURN(drawSyntaxTreeRecursively(dumper, tree, node.right, nodeInd, settings));

    return DUMPER_STATUS_OK;
}

char* getLastImageFileName(const Dumper* dumper) {
    assert(dumper != NULL);

    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE,
            "%zu_list.%s", dumper->numberOfLogsBefore, dumper->outputFileFormat);

    snprintf(tmpBuffer, TMP_BUFFER_SIZE,
             "%s/images/%s", dumper->dirForLogsPath, fileNameBuffer);
    return tmpBuffer;
}



DumperErrors dumperDumpSyntaxTree(
    Dumper*               dumper,
    const SyntaxTree*     tree,
    const DumperSettings* settings
) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(settings);
    IF_ARG_NULL_RETURN(tree);

    LOG_DEBUG("syntax tree dumping ---------------------");
    ++dumper->numberOfLogsBefore;
    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE, "dots/%zu_list.dot",
             dumper->numberOfLogsBefore);

    memset(fileFullNameBuffer, 0, FULL_FILE_NAME_BUFFER_SIZE);
    snprintf(fileFullNameBuffer, FULL_FILE_NAME_BUFFER_SIZE,
             "%s/%s", dumper->dirForLogsPath, fileNameBuffer);

    LOG_DEBUG_VARS(fileFullNameBuffer, fileNameBuffer);
    FILE* outputFile = fopen(fileFullNameBuffer, "w");
    IF_NOT_COND_RETURN(outputFile != NULL,
                       DUMPER_ERROR_COULD_OPEN_FILE);

    memset(buffer, 0, BUFFER_SIZE);
    // FIXME: add errors check
    strncat(buffer,
        "digraph html {\n"
        "overlap=false\n"
        "bgcolor=\"black\"\n"
        "rankdir=TB\n"
        "pad=0.2\n\
        ", BUFFER_SIZE);

    IF_ERR_RETURN(drawSyntaxTreeRecursively(dumper, tree, tree->root, 0, settings));

    strncat(buffer, "}\n", BUFFER_SIZE);
    fprintf(outputFile, buffer);
    fclose(outputFile);

    tmpBuffer = getLastImageFileName(dumper);

    // TODO: put assert for ;
    memset(fileFullNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileFullNameBuffer, FULL_FILE_NAME_BUFFER_SIZE,
             "dot -Tpng logs/dots/%zu_list.dot -o %s",
             dumper->numberOfLogsBefore, tmpBuffer);
    LOG_DEBUG_VARS(fileFullNameBuffer);
    // WARNING: some nasty command can be substituted
    system(fileFullNameBuffer);

    memset(fileFullNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileFullNameBuffer, FULL_FILE_NAME_BUFFER_SIZE,
             "images/%s", fileNameBuffer);
    LOG_DEBUG_VARS(fileFullNameBuffer);
    dumperAddImgToAllLogsFile(dumper, fileFullNameBuffer);

    return DUMPER_STATUS_OK;
}

DumperErrors dumperDestructor(Dumper* dumper) {
    IF_ARG_NULL_RETURN(dumper);

    dumperAddDebugInfoToAllLogsFile(dumper, "</body>\n");

    FREE(fileNameBuffer);
    FREE(tmpBuffer);
    FREE(buffer);
    FREE(fileFullNameBuffer);

    fclose(dumper->allLogsFile);
    *dumper = {};

    return DUMPER_STATUS_OK;
}
