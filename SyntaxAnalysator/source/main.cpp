#include "../../external/LoggerLib/include/logLib.hpp"
#include "../include/syntaxAnalysator.hpp"
#include "../../Dumper/include/dumper.hpp"

int main() {
    setLoggingLevel(DEBUG);

    exit(0);
    Dumper dumper = {};
    dumperConstructor(&dumper, 10, "logs", "png");

    const char* sourceFilePath = "../LexemAnalysator/dest.txt";
    const char*   destFilePath = "dest.txt";

    SyntaxAnalysator analysator = {};
    constructSyntaxAnalysator(sourceFilePath, destFilePath, &analysator, &dumper);
    readArrOfLexemsFromFile(&analysator);

    dumpSyntaxAnalysatorArrOfLexems(&analysator);

    dumpSyntaxAnalysatorArrOfLexems(&analysator);
    generateSyntaxTree(&analysator);

    dumpSyntaxTreeInConsole(&analysator.tree);
    openImageOfCurrentStateSyntaxTree(&analysator.tree);

    saveAnalysatorSyntaxTree2File(&analysator);

    destructSyntaxAnalysator(&analysator);
    dumperDestructor(&dumper);

    return 0;
}
