#ifndef CODE_GENERATOR_INCLUDE_CODE_GENERATOR_HPP
#define CODE_GENERATOR_INCLUDE_CODE_GENERATOR_HPP

#include "codeGeneratorErrorsHandler.hpp"
#include "../../SyntaxTree/include/syntaxTree.hpp"
#include "../../SemanticChecker/include/semanticChecker.hpp"

struct CodeGenerator {
    const char*             sourceFilePath;
    const char*             destFilePath;
    SyntaxTree              tree;
    Dumper*                 dumper;
    SemanticChecker         checker;
};

CodeGeneratorErrors constructCodeGenerator(
    const char*         sourceFilePath,
    const char*         destFilePath,
    Dumper*             dumper,
    CodeGenerator*      generator
);
CodeGeneratorErrors readCodeGeneratorSyntaxTreeFromFile(CodeGenerator* generator);
CodeGeneratorErrors generateAssemblerCodeFromSyntaxTree(CodeGenerator* generator);
CodeGeneratorErrors destructCodeGenerator(CodeGenerator* generator);

#endif
