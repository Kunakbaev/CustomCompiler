#include "../../external/LoggerLib/include/logLib.hpp"
#include "../include/syntaxAnalysator.hpp"
#include "../../Dumper/include/dumper.hpp"

int main() {
    setLoggingLevel(DEBUG);

    Dumper dumper = {};
    dumperConstructor(&dumper, 10, "logs", "png");

    const char* sourceFilePath = "../LexemAnalysator/dest.txt";
    const char*   destFilePath = "dest.txt";

    SyntaxAnalysator analysator = {};
    constructSyntaxAnalysator(sourceFilePath, destFilePath, &analysator, &dumper);
    readArrOfLexemsFromFile(&analysator);

    dumpSyntaxAnalysatorArrOfLexems(&analysator);
    generateSyntaxTree(&analysator);

    dumpSyntaxTreeInConsole(&analysator.tree);
    dumpSyntaxTree(&analysator.tree);
    LOG_ERROR("main");
    openImageOfCurrentStateSyntaxTree(&analysator.tree);
    saveAnalysatorSyntaxTree2File(&analysator);

    destructSyntaxAnalysator(&analysator);
    dumperDestructor(&dumper);

    return 0;
}
