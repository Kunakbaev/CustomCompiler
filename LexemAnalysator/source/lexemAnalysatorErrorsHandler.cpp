#include "../include/lexemAnalysatorErrorsHandler.hpp"

const char* getLexemAnalysatorErrorMessage(LexemAnalysatorErrors error) {
    #define ERROR_UNPACK(errName, errCode, errMessage)          \
        case LEXEM_ANALYSATOR_##errName:                                           \
            return "Lexem analysator error: " errMessage ".\n";    \

    switch (error) {
        case LEXEM_ANALYSATOR_STATUS_OK:
            return "Lexem analysator: no errors occured.\n";

        #include "../include/errorsPlainText.in"

        default:
            return "Lexem analysator error: unknown error.\n";
    }

    #undef UNPACK
}
