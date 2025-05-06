#include "../../external/LoggerLib/include/logLib.hpp"
#include "../include/codeGenerator.hpp"
#include "../../Dumper/include/dumper.hpp"
#include "../../SemanticChecker/include/semanticChecker.hpp"

int main() {
    setLoggingLevel(DEBUG);

    Dumper dumper = {};
    dumperConstructor(&dumper, 10, "logs", "png");

    const char* sourceFilePath = "../SyntaxAnalysator/dest.txt";
    const char*   destFilePath = "dest.asm";

    CodeGenerator generator = {};
    constructCodeGenerator(sourceFilePath, destFilePath, &dumper, &generator);
    readCodeGeneratorSyntaxTreeFromFile(&generator);
    openImageOfCurrentStateSyntaxTree(&generator.tree);

    semanticCheckOfSyntaxTree(&generator.checker);
    generateAssemblerCodeFromSyntaxTree(&generator);

    destructCodeGenerator(&generator);
    dumperDestructor(&dumper);

    return 0;
}
