#include "../include/genMyCustomAsmErrorsHandler.hpp"

const char* getGenMyCustomAsmErrorMessage(GenMyCustomAsmErrors error) {
    #define ERROR_UNPACK(errName, errCode, errMessage)          \
        case GEN_MY_CUSTOM_ASM_##errName:                                           \
            return "Lexem analysator error: " errMessage ".\n";    \

    switch (error) {
        case GEN_MY_CUSTOM_ASM_STATUS_OK:
            return "Intermid repr: no errors occured.\n";

        #include "../include/errorsPlainText.txt"

        default:
            return "Intermid repr error: unknown error.\n";
    }

    #undef UNPACK
}
