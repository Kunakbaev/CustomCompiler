#include "../include/semanticCheckerErrorsHandler.hpp"

const char* getSemanticCheckerErrorMessage(SemanticCheckerErrors error) {
    #define ERROR_UNPACK(errName, errCode, errMessage)          \
        case SEMANTIC_CHECKER_##errName:                                           \
            return "Semantic checker error: " errMessage ".\n";    \

    switch (error) {
        case SEMANTIC_CHECKER_STATUS_OK:
            return "Semantic checker: no errors occured.\n";

        // ASK: code generation
        #include "../include/errorsPlainText.in"

        default:
            return "Semantic checker error: unknown error.\n";
    }

    #undef UNPACK
}
