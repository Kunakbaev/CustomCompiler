#include "../include/codeGeneratorErrorsHandler.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, CODE_GENERATOR_INVALID_ARGUMENT, getCodeGeneratorErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getCodeGeneratorErrorMessage, CODE_GENERATOR_STATUS_OK);\

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getCodeGeneratorErrorMessage)\

#define DUMPER_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getDumperErrorMessage, DUMPER_STATUS_OK, SYNTAX_ANALYSATOR_DUMPER_ERROR);

#define LEXEMS_REALIZATIONS_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getLexemsRealizationsErrorMessage, LEXEMS_REALIZATIONS_STATUS_OK, CODE_GENERATOR_LEXEMS_REALIZATIONS_ERROR);
