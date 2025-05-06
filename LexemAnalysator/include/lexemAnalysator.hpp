#ifndef LEXEM_ANALYSATOR_INCLUDE_LEXEM_ANALYSATOR_HPP
#define LEXEM_ANALYSATOR_INCLUDE_LEXEM_ANALYSATOR_HPP

#include "../../SyntaxTree/include/syntaxTreeNodeStruct.hpp"
#include "lexemAnalysatorErrorsHandler.hpp"

struct LexemAnalysator {
    const char*     sourceFilePath;
    const char*     destFilePath;
    size_t          arrLen;
    Lexem*          array;
    char*     inputString; // TODO: how to make it const
    size_t          inputStringLen;
};

LexemAnalysatorErrors constructLexemAnalysator(const char* sourceFilePath,
                                               const char* destFilePath,
                                               LexemAnalysator* analysator);
LexemAnalysatorErrors dumpLexemAnalysator(LexemAnalysator* analysator);
LexemAnalysatorErrors getArrayOfLexems(LexemAnalysator* analysator);
LexemAnalysatorErrors openCurImageOfLexemAnalysator(LexemAnalysator* analysator);
LexemAnalysatorErrors processSourceFile(LexemAnalysator* analysator);
LexemAnalysatorErrors saveArrayOfLexems2File(LexemAnalysator* analysator);
LexemAnalysatorErrors destructLexemAnalysator(LexemAnalysator* analysator);

#endif
