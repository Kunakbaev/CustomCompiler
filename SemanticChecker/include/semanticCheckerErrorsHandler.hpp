#ifndef SEMANTIC_VALIDATION_INCLUDE_SEMANTIC_VALIDATION_ERRORS_HPP
#define SEMANTIC_VALIDATION_INCLUDE_SEMANTIC_VALIDATION_ERRORS_HPP

#include "../../common/include/errorsHandlerDefines.hpp"

#define ERROR_UNPACK(errName, errCode, errMessage) \
    SEMANTIC_CHECKER_##errName = errCode,

enum SemanticCheckerErrors {
    SEMANTIC_CHECKER_STATUS_OK = 0,
    #include "errorsPlainText.in"
};

#undef ERROR_UNPACK

const char* getSemanticCheckerErrorMessage(SemanticCheckerErrors error);

#endif
