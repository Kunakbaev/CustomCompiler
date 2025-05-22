#include "../include/intermidRepr.hpp"
#include "../../GenAsmForMyProcessor/include/genMyCustomAsm.hpp"

const char* NASM_OPTION_ARG_NAME = "nasm";

int main(int argc, const char* argv[]) {
    setLoggingLevel(DEBUG);
    assert(argc >= 2);

    LOG_DEBUG_VARS(argv[0], argv[1], NASM_OPTION_ARG_NAME);
    bool isNasm = strcmp(argv[1], NASM_OPTION_ARG_NAME) == 0;

    LOG_DEBUG("Hello!");

    Dumper dumper = {};
    dumperConstructor(&dumper, 10, "logs", "png");

    const char* sourceFilePath = "../SyntaxAnalysator/dest.txt";
    const char*   destFilePath = "dest.txt";

    IntermidRepr intermidRepr = {};
    constructIntermidReprFromSyntaxTree(sourceFilePath, &dumper, &intermidRepr);
    readIntermidReprSyntaxTreeFromFile(&intermidRepr);

    getIntermidRepresentation(&intermidRepr);
    dumpTextVersionOfIntermidRepr2file(&intermidRepr, destFilePath);

    genMyCustomAsmAndSaveIt2File(
        &intermidRepr,
        "asmDestFile.asm",
        !isNasm
    );

    destructIntermidRepr(&intermidRepr);

    return 0;
}