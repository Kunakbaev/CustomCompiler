#ifndef INTERMEDIATE_REPRESENTATION_INCLUDE_INTERMID_REPR_HPP
#define INTERMEDIATE_REPRESENTATION_INCLUDE_INTERMID_REPR_HPP

#include <cstddef>

#include "intermidReprErrorsHandler.hpp"
#include "../../SyntaxTree/include/syntaxTree.hpp"

enum ArgumentType {
    INVALID_ARGUMENT_TYPE       = 0,
    NUMBER_ARGUMENT_TYPE        = 1,
    REGISTER_ARGUMENT_TYPE      = 2,
    ADDRESS_ARGUMENT_TYPE       = 3, // for jumps and functions calls
};

struct Command {
    int         commandCode;
    size_t      numOfArgs;
};

struct IntermidRepr {
    size_t   numOfCommands;
    Command* commands;
};

IntermidReprErrors constructIntermidReprFromSyntaxTree(
    const SyntaxTree* tree,
    IntermidRepr*     intermidRepr
);

#endif