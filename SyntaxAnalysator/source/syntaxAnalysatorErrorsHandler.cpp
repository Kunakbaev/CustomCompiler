#include "../include/syntaxAnalysatorErrorsHandler.hpp"

const char* getSyntaxAnalysatorErrorMessage(SyntaxAnalysatorErrors error) {
    #define ERROR_UNPACK(errName, errCode, errMessage)              \
        case SYNTAX_ANALYSATOR_##errName:                           \
            return "Syntax analysator error: " errMessage ".\n";    \

    switch (error) {
        case SYNTAX_ANALYSATOR_STATUS_OK:
            return "Syntax analysator: no errors occured.\n";

        #include "../include/errorsPlainText.in"

        default:
            return "Syntax analysator error: unknown error.\n";
    }

    #undef UNPACK
}
