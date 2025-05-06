#include "../../external/LoggerLib/include/logLib.hpp"
#include "../include/lexemAnalysator.hpp"

int main() {
    setLoggingLevel(DEBUG);

    LexemAnalysator analysator = {};
    constructLexemAnalysator("source.txt", "dest.txt", &analysator);
    processSourceFile(&analysator);
    getArrayOfLexems(&analysator);
    saveArrayOfLexems2File(&analysator);

    return 0;
}
