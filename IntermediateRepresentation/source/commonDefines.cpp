
#define GEN4LEFT() \
    IF_ERR_RETURN(parseFunctionRecursive(intermidRepr, function, node->left, depthInBlocksOfCode));

#define GEN4RIGHT() \
    IF_ERR_RETURN(parseFunctionRecursive(intermidRepr, function, node->right, depthInBlocksOfCode));

#define ADD_COMMAND(commandType, argumentField, argumentValue)                              \
do {                                                                                        \
    IntermidReprCommandArgument argument = {};                                              \
    argument.argumentField = argumentValue;                                               \
    IF_ERR_RETURN(addNewCommandToFuncRepr(function, commandType, argument, depthInBlocksOfCode));                \
} while (0)

#define ADD_COMMAND_NO_ARGS(commandType) ADD_COMMAND(commandType, intNumber, 0)

#define ADD_LABEL()                                                                         \
do {                                                                                        \
    IF_ERR_RETURN(addNewLabelToFuncRepr(function, depthInBlocksOfCode));                                         \
} while (0)
