#include "../include/syntaxTreeErrorsHandler.hpp"

const char* getSyntaxTreeErrorMessage(SyntaxTreeErrors error) {
    #define ERROR_UNPACK(errName, errCode, errMessage)        \
        case SYNTAX_TREE_##errName:                           \
            return "Syntax tree error: " errMessage ".\n";    \

    switch (error) {
        case SYNTAX_TREE_STATUS_OK:
            return "Syntax tree: no errors occured.\n";

        #include "../include/errorsPlainText.in"

        default:
            return "Syntax tree error: unknown error.\n";
    }

    #undef UNPACK
}
