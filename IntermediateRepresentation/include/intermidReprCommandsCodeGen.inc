INTERMID_REPR_CMD_DEF("?",              INVALID_INTERMID_REPR_CMD             , 0)
INTERMID_REPR_CMD_DEF("push",           PUSH_NUM_INTERMID_REPR_CMD            , 1)
INTERMID_REPR_CMD_DEF("push",           PUSH_VAR_INTERMID_REPR_CMD            , 2)
INTERMID_REPR_CMD_DEF("pop",            POP_VAR_INTERMID_REPR_CMD             , 3)
INTERMID_REPR_CMD_DEF("add",            ADD_INTERMID_REPR_CMD                 , 4)
INTERMID_REPR_CMD_DEF("sub",            SUB_INTERMID_REPR_CMD                 , 5)
INTERMID_REPR_CMD_DEF("mul",            MUL_INTERMID_REPR_CMD                 , 6)
INTERMID_REPR_CMD_DEF("div",            DIV_INTERMID_REPR_CMD                 , 7)
INTERMID_REPR_CMD_DEF("jmp",            UNCOND_JMP_INTERMID_REPR_CMD          , 8)
INTERMID_REPR_CMD_DEF("je",             IF_EQ_JMP_INTERMID_REPR_CMD           , 9)
INTERMID_REPR_CMD_DEF("enter",          ENTER_INTERMID_REPR_CMD               , 10)
INTERMID_REPR_CMD_DEF("leave",          LEAVE_INTERMID_REPR_CMD               , 11)
INTERMID_REPR_CMD_DEF("return",         RETURN_INTERMID_REPR_CMD              , 12)
// for now not implemented, maybe latter I will change calling convention
// from passing all arguments through stack to smth more complex.
// than, this function will come in handy, but for now it would be just a push command copy
INTERMID_REPR_CMD_DEF("paramIn",        PARAM_IN_NUM_INTERMID_REPR_CMD        , 13)
INTERMID_REPR_CMD_DEF("paramIn",        PARAM_IN_VAR_INTERMID_REPR_CMD        , 14)
// at first I've wanted to use simple pop instruction, however for nasm 
INTERMID_REPR_CMD_DEF("paramOut",       PARAM_OUT_INTERMID_REPR_CMD           , 15)
INTERMID_REPR_CMD_DEF("call",           CALL_INTERMID_REPR_CMD                , 16)
INTERMID_REPR_CMD_DEF("input",          INPUT_INTERMID_REPR_CMD               , 17)
INTERMID_REPR_CMD_DEF("output",         OUTPUT_INTERMID_REPR_CMD              , 18)
INTERMID_REPR_CMD_DEF("and",            AND_INTERMID_REPR_CMD                 , 19)
INTERMID_REPR_CMD_DEF("or",             OR_INTERMID_REPR_CMD                  , 20)
INTERMID_REPR_CMD_DEF("lessCmp",        IS_LESS_INTERMID_REPR_CMD             , 21)
INTERMID_REPR_CMD_DEF("greaterCmp",     IS_GREATER_INTERMID_REPR_CMD          , 22)
INTERMID_REPR_CMD_DEF("lessOrEqCmp",    IS_LESS_OR_EQ_INTERMID_REPR_CMD       , 23)
INTERMID_REPR_CMD_DEF("greaterOrEqCmp", IS_GREATER_OR_EQ_INTERMID_REPR_CMD    , 24)
INTERMID_REPR_CMD_DEF("isEqCmp",        IS_EQ_INTERMID_REPR_CMD               , 25)
