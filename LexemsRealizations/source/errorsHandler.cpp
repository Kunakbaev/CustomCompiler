#include "../include/errorsHandler.hpp"

const char* getLexemsRealizationsErrorMessage(LexemsRealizationsErrors error) {
    #define ERROR_UNPACK(errName, errCode, errMessage)             \
        case LEXEMS_REALIZATIONS_##errName:                           \
            return "Lexems realizations error: " errMessage ".\n";    \

    switch (error) {
        case LEXEMS_REALIZATIONS_STATUS_OK:
            return "Lexems realizations: no errors occured.\n";

        #include "../include/errorsPlainText.in"

        default:
            return "Lexems realizations error: unknown error.\n";
    }

    #undef UNPACK
}
