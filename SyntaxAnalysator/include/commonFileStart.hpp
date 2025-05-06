#include "../include/syntaxAnalysator.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, SYNTAX_ANALYSATOR_INVALID_ARGUMENT, getSyntaxAnalysatorErrorMessage)

#define IF_ERR_RETURN(error) do {\
        SyntaxAnalysatorErrors errorTmp = (error); \
        if (errorTmp != SYNTAX_ANALYSATOR_STATUS_OK) {\
            LOG_ERROR(getSyntaxAnalysatorErrorMessage(errorTmp));\
            return errorTmp;\
        }\
    } while (0)
//COMMON_IF_ERR_RETURN(error, getSyntaxAnalysatorErrorMessage, SYNTAX_ANALYSATOR_STATUS_OK);\

#define IF_NOT_COND_RETURN(condition, error) \
    do {\
        bool tmpCond = (condition);\
        if (!tmpCond) {\
            LOG_ERROR(#condition);\
            LOG_ERROR("condition not satisfied"); LOG_ERROR(#condition);\
            return error;\
        }\
    } while (0)
//COMMON_IF_NOT_COND_RETURN(condition, error, getSyntaxAnalysatorErrorMessage)\

#define DUMPER_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getDumperErrorMessage, DUMPER_STATUS_OK, SYNTAX_ANALYSATOR_DUMPER_ERROR);

#define LEXEMS_REALIZATIONS_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getLexemsRealizationsErrorMessage, LEXEMS_REALIZATIONS_STATUS_OK, SYNTAX_ANALYSATOR_LEXEMS_REALIZATIONS_ERROR);
