#ifndef CODE_GENERATOR_INCLUDE_CODE_GENERATOR_ERRORS_HANDLER_HPP
#define CODE_GENERATOR_INCLUDE_CODE_GENERATOR_ERRORS_HANDLER_HPP

#define ERROR_UNPACK(enumName, ...) \
    CODE_GENERATOR_##enumName,

enum CodeGeneratorErrors {
    CODE_GENERATOR_STATUS_OK = 0,
    #include "errorsPlainText.in"
};

#undef ERROR_UNPACK

const char* getCodeGeneratorErrorMessage(CodeGeneratorErrors error);

#endif
