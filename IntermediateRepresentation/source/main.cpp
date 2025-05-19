#include "../include/intermidRepr.hpp"
#include "../../GenAsmForMyProcessor/include/genMyCustomAsm.hpp"

int main() {
    setLoggingLevel(DEBUG);

    LOG_DEBUG("Hello!");

    Dumper dumper = {};
    dumperConstructor(&dumper, 10, "logs", "png");

    const char* sourceFilePath = "../SyntaxAnalysator/dest.txt";
    const char*   destFilePath = "dest.txt";

    IntermidRepr intermidRepr = {};
    constructIntermidReprFromSyntaxTree(sourceFilePath, &dumper, &intermidRepr);
    LOG_ERROR("ok1");
    readIntermidReprSyntaxTreeFromFile(&intermidRepr);
    LOG_ERROR("ok2");

    getIntermidRepresentation(&intermidRepr);
    LOG_ERROR("ok3");

    dumpTextVersionOfIntermidRepr2file(&intermidRepr, destFilePath);

    genMyCustomAsmAndSaveIt2File(
        &intermidRepr,
        "asmDestFile.asm",
        false
    );

    destructIntermidRepr(&intermidRepr);

    return 0;
}