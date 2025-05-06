#include "../include/codeGeneratorErrorsHandler.hpp"

const char* getCodeGeneratorErrorMessage(CodeGeneratorErrors error) {
    #define ERROR_UNPACK(errName, errCode, errMessage)          \
        case CODE_GENERATOR_##errName:                                           \
            return "Code generator error: " errMessage ".\n";    \

    switch (error) {
        case CODE_GENERATOR_STATUS_OK:
            return "Code generator: no errors occurred.\n";

        #include "../include/errorsPlainText.in"

        default:
            return "Code generator error: unknown error.\n";
    }

    #undef UNPACK
}
