#ifndef LEXEMS_REALIZATIONS_INCLUDE_LEXEMS_REALIZATIONS_HPP
#define LEXEMS_REALIZATIONS_INCLUDE_LEXEMS_REALIZATIONS_HPP

#include "errorsHandler.hpp"
//#include "../../Dumper/include/dumperStruct.hpp"
//#include "../../SyntaxTree/include/syntaxTree.hpp"

enum LexemType {
    INVALID_LEXEM_TYPE       = 0,
    CONST_LEXEM_TYPE         = 1,
    IDENTIFICATOR_LEXEM_TYPE = 2,
    KEYWORD_LEXEM_TYPE       = 3,
    DELIM_LEXEM_TYPE         = 4,
    OPERATOR_LEXEM_TYPE      = 5,
};

#define GENERAL_LEXEM_DEF(_, enumName, ...) \
    enumName,

enum Lexems {
    INVALID_LEXEM,
    #include "codeGen/allLexems.hpp"
};

#undef GENERAL_LEXEM_DEF

struct Lexem {
    LexemType   type;
    char* strRepr; // str repr can be get from data (var index, or operator index or keyword index), so we would just have table with names and addition getter funcs
    // general lexem identificator or number (const)
    union {
        Lexems   lexemSpecificName;
        double   doubleData;
    };
};

#undef GENERAL_LEXEM_DEF

#define GENERAL_LEXEM_DEF(lexemType, specName, repr) \
    const Lexem SAMPLE_##specName##_NODE = {.type=lexemType, .strRepr = repr, {.lexemSpecificName = specName}};

#include "../include/codeGen/allLexems.hpp"

#undef GENERAL_LEXEM_DEF

struct Lexem2stringSettings {
    bool isLexemTypeNeeded;
    bool isBracketsNeeded;
};

const char* getLexemTypeString(LexemType type);
LexemsRealizationsErrors readLexemFromFile(
    FILE*        file,
    Lexem*       lexem,
    char*        lineBuffer,
    const size_t maxLineLen
);
LexemsRealizationsErrors initLexemWithString(const char*  line,  Lexem*       lexem);
LexemsRealizationsErrors    isCharLexemDelim(const char   ch,    bool*        isDelim);
LexemsRealizationsErrors getLexemDebugString(const Lexem* lexem, char**       result);
LexemsRealizationsErrors     saveLexemToFile(FILE*        file,  const Lexem* lexem);
LexemsRealizationsErrors  getLexemDataString(
    const Lexem*                lexem,
    const Lexem2stringSettings* settings, 
    char**                      line);

#endif
