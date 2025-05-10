#include "../include/intermidReprErrorsHandler.hpp"

const char* getIntermidReprErrorMessage(IntermidReprErrors error) {
    #define ERROR_UNPACK(errName, errCode, errMessage)          \
        case INTERMID_REPR_##errName:                                           \
            return "Lexem analysator error: " errMessage ".\n";    \

    switch (error) {
        case INTERMID_REPR_STATUS_OK:
            return "Intermid repr: no errors occured.\n";

        #include "../include/errorsPlainText.txt"

        default:
            return "Intermid repr error: unknown error.\n";
    }

    #undef UNPACK
}
