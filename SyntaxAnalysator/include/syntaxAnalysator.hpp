#ifndef SYNTAX_ANALYSATOR_INCLUDE_SYNTAX_ANALYSATOR_HPP
#define SYNTAX_ANALYSATOR_INCLUDE_SYNTAX_ANALYSATOR_HPP

#include "../../LexemsRealizations/include/lexemsRealizations.hpp"
#include "../../SyntaxTree/include/syntaxTree.hpp"
#include "syntaxAnalysatorErrorsHandler.hpp"
#include "../../Dumper/include/dumper.hpp"

struct SyntaxAnalysator {
    const char*         sourceFilePath;
    const char*         destFilePath;
    Lexem*              arrOfLexems;
    size_t              lenOfArr;
    SyntaxTree          tree;
    size_t              curLexemInd;
    Dumper*             dumper;
};

SyntaxAnalysatorErrors constructSyntaxAnalysator(
    const char*       sourceFilePath,
    const char*       destFilePath,
    SyntaxAnalysator* analysator,
    Dumper*           dumper
);
bool compareOpRule(Lexem lexem);
SyntaxAnalysatorErrors dumpSyntaxAnalysatorTreeInConsole(const SyntaxAnalysator* analysator);
SyntaxAnalysatorErrors   dumpSyntaxAnalysatorArrOfLexems(const SyntaxAnalysator* analysator);
SyntaxAnalysatorErrors           readArrOfLexemsFromFile(      SyntaxAnalysator* analysator);
SyntaxAnalysatorErrors                generateSyntaxTree(      SyntaxAnalysator* analysator);
SyntaxAnalysatorErrors     saveAnalysatorSyntaxTree2File(      SyntaxAnalysator* analysator);
SyntaxAnalysatorErrors          destructSyntaxAnalysator(      SyntaxAnalysator* analysator);

#endif
