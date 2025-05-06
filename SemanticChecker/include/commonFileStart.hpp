#include "semanticCheckerErrorsHandler.hpp"
#include "semanticChecker.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, SEMANTIC_CHECKER_INVALID_ARGUMENT, getSemanticCheckerErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getSemanticCheckerErrorMessage, SEMANTIC_CHECKER_STATUS_OK);\

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getSemanticCheckerErrorMessage)\

#define DUMPER_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getDumperErrorMessage, DUMPER_STATUS_OK, SEMANTIC_CHECKER_DUMPER_ERROR);

#define LEXEMS_REALIZATIONS_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getLexemsRealizationsErrorMessage, LEXEMS_REALIZATIONS_STATUS_OK, SEMANTIC_CHECKER_LEXEMS_REALIZATIONS_ERROR);
