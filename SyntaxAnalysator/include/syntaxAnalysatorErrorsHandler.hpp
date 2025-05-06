#ifndef SYNTAX_ANALYSATOR_INCLUDE_SYNTAX_ANALYSATOR_ERRORS_HANDLER_HPP
#define SYNTAX_ANALYSATOR_INCLUDE_SYNTAX_ANALYSATOR_ERRORS_HANDLER_HPP

#define ERROR_UNPACK(enumName, ...) \
    SYNTAX_ANALYSATOR_##enumName,

enum SyntaxAnalysatorErrors {
    SYNTAX_ANALYSATOR_STATUS_OK = 0,
    #include "errorsPlainText.in"
};

#undef ERROR_UNPACK

const char* getSyntaxAnalysatorErrorMessage(SyntaxAnalysatorErrors error);

#endif
