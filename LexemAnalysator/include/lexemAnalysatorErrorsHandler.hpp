#ifndef LEXEM_ANALYSATOR_INCLUDE_ERRORS_HANDLER_ERRORS_HPP
#define LEXEM_ANALYSATOR_INCLUDE_ERRORS_HANDLER_ERRORS_HPP

#include "../../common/include/errorsHandlerDefines.hpp"

#define ERROR_UNPACK(errName, errCode, errMessage) \
    LEXEM_ANALYSATOR_##errName = errCode,

enum LexemAnalysatorErrors {
    LEXEM_ANALYSATOR_STATUS_OK             = 0,
    #include "errorsPlainText.in"
};

#undef ERROR_UNPACK

const char* getLexemAnalysatorErrorMessage(LexemAnalysatorErrors error);

#endif
