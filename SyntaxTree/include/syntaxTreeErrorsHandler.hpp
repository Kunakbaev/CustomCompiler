#ifndef SYNTAX_TREE_INCLUDE_ERRORS_HANDLER_HPP
#define SYNTAX_TREE_INCLUDE_ERRORS_HANDLER_HPP

#include "../../common/include/errorsHandlerDefines.hpp"

#define ERROR_UNPACK(enumName, errorCode, _) \
    SYNTAX_TREE_##enumName = errorCode,

enum SyntaxTreeErrors {
    SYNTAX_TREE_STATUS_OK = 0,
    #include "errorsPlainText.in"
};

#undef ERROR_UNPACK

const char* getSyntaxTreeErrorMessage(SyntaxTreeErrors error);

#endif
