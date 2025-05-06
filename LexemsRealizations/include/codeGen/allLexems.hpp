#define    DELIM_LEXEM_DEF(...) GENERAL_LEXEM_DEF(DELIM_LEXEM_TYPE,    __VA_ARGS__)
#define  KEYWORD_LEXEM_DEF(...) GENERAL_LEXEM_DEF(KEYWORD_LEXEM_TYPE,  __VA_ARGS__)
#define OPERATOR_LEXEM_DEF(...) GENERAL_LEXEM_DEF(OPERATOR_LEXEM_TYPE, __VA_ARGS__)

#include "delimsLexems.in"
#include "keywordsLexems.in"
#include "operatorsLexems.in"

#undef DELIM_LEXEM_DEF
#undef KEYWORD_LEXEM_DEF
#undef OPERATOR_LEXEM_DEF
