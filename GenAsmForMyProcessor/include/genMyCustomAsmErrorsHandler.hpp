#ifndef GEN_ASM_FOR_MY_PROCESSOR_GEN_MY_CUSTOM_ASM_ERRORS_HANDLER_HPP
#define GEN_ASM_FOR_MY_PROCESSOR_GEN_MY_CUSTOM_ASM_ERRORS_HANDLER_HPP

#include "../../common/include/errorsHandlerDefines.hpp"

#define ERROR_UNPACK(errName, errCode, errMessage) \
    GEN_MY_CUSTOM_ASM_##errName = errCode,

enum GenMyCustomAsmErrors {
    GEN_MY_CUSTOM_ASM_STATUS_OK             = 0,
    #include "errorsPlainText.txt"
};

#undef ERROR_UNPACK

const char* getGenMyCustomAsmErrorMessage(GenMyCustomAsmErrors error);

#endif