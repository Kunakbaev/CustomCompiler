#ifndef INTERMEDIATE_REPRESENTATION_INCLUDE_INTERMID_REPR_ERRORS_HANDLER_HPP
#define INTERMEDIATE_REPRESENTATION_INCLUDE_INTERMID_REPR_ERRORS_HANDLER_HPP

#include "../../common/include/errorsHandlerDefines.hpp"

#define ERROR_UNPACK(errName, errCode, errMessage) \
    INTERMID_REPR_##errName = errCode,

enum IntermidReprErrors {
    INTERMID_REPR_STATUS_OK             = 0,
    #include "errorsPlainText.txt"
};

#undef ERROR_UNPACK

const char* getIntermidReprErrorMessage(IntermidReprErrors error);

#endif