#ifndef INTERMEDIATE_REPRESENTATION_INCLUDE_INTERMID_REPR_HPP
#define INTERMEDIATE_REPRESENTATION_INCLUDE_INTERMID_REPR_HPP

#include <cstddef>

#include "intermidReprErrorsHandler.hpp"
#include "../../Dumper/include/dumper.hpp"
#include "../../SyntaxTree/include/syntaxTree.hpp"
#include "../../SemanticChecker/include/semanticChecker.hpp"

// enum ArgumentType {
//     INVALID_ARGUMENT_TYPE       = 0,
//     NUMBER_ARGUMENT_TYPE        = 1,
//     REGISTER_ARGUMENT_TYPE      = 2,
//     ADDRESS_ARGUMENT_TYPE       = 3, // for jumps and functions calls
// };

#define INTERMID_REPR_CMD_DEF(strRepr, enumName, id) \
    enumName = id,

enum IntermidReprCommandType {
    #include "intermidReprCommandsCodeGen.inc"
};

struct CommandTypeAndStrReprPair {
    IntermidReprCommandType commandType;
    const char*             strRepr;
};

#undef INTERMID_REPR_CMD_DEF

union IntermidReprCommandArgument {
    int                 jumpDestLabelInd;
    size_t              variableInd;
    double              doubleNumber;
    int                 intNumber;
    const char*         functionName;
};

struct IntermidReprCommand {
    IntermidReprCommandType     commandType;
    IntermidReprCommandArgument argument;
};

struct IntermidReprLabel {
    size_t labelInd;
};

struct IntermidReprElement {
    bool   isCommand;
    size_t depthInBlocksOfCode;
    union {
        IntermidReprCommand command;
        IntermidReprLabel   label;
    };
    IntermidReprElement* prevElement;
    IntermidReprElement* nextElement;
};

struct LocalVarsListNode {
    char*              name;
    LocalVarsListNode* prev;
};

struct FunctionRepr {
    size_t                  numOfElements;
    int                     numOfLocalVars;
    char*                   functionName;
    IntermidReprElement*    listHead; // pointer to the head of linked list
    IntermidReprElement*    listTail; // pointer to the tail of linked list
    LocalVarsListNode*      localVarsListTail;
    FunctionRepr*           prevFunc;          
    FunctionRepr*           nextFunc;          
    // TODO: return type, for now it's just int
};

struct IntermidRepr {
    const char*      sourceFilePath;
    FunctionRepr*    funcReprListHead; // pointer to the head of linked list
    FunctionRepr*    funcReprListTail; // pointer to the tail of linked list
    SemanticChecker  checker;
    SyntaxTree       tree;
};

IntermidReprErrors getCommandStringRepr(
    IntermidReprCommandType commandType,
    const char**            result
);

IntermidReprErrors constructIntermidReprFromSyntaxTree(
    const char*       sourceFilePath,
    Dumper*           dumper,
    IntermidRepr*     intermidRepr
);

IntermidReprErrors readIntermidReprSyntaxTreeFromFile(
    IntermidRepr* intermidRepr
);

IntermidReprErrors getIntermidRepresentation(
    IntermidRepr* intermidRepr
);

IntermidReprErrors dumpTextVersionOfIntermidRepr2file(
    IntermidRepr* intermidRepr,
    const char*   destFilePath
);

IntermidReprErrors dumpArrayVersionOfIntermidRepr(
    IntermidRepr* intermidRepr
);

IntermidReprErrors destructIntermidRepr(
    IntermidRepr* intermidRepr 
);

#endif