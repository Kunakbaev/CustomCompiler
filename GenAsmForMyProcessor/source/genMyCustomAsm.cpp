#include "../include/genMyCustomAsm.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, GEN_MY_CUSTOM_ASM_INVALID_ARGUMENT, getGenMyCustomAsmErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getGenMyCustomAsmErrorMessage, GEN_MY_CUSTOM_ASM_STATUS_OK);

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getGenMyCustomAsmErrorMessage)

#define INTERMID_REPR_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getIntermidReprErrorMessage, INTERMID_REPR_STATUS_OK, GEN_MY_CUSTOM_ASM_INTERMID_REPR_ERROR)

static GenMyCustomAsmErrors addMyAsmCommandStrRepr2file(
    const FunctionRepr*        funcRepr,
    const IntermidReprElement* elemPtr,
    const char*                tabsBuffer,
    FILE*                      file
) {
    IF_ARG_NULL_RETURN(funcRepr);
    IF_ARG_NULL_RETURN(elemPtr);
    IF_ARG_NULL_RETURN(tabsBuffer);
    IF_ARG_NULL_RETURN(file);
    IF_NOT_COND_RETURN(elemPtr->isCommand, GEN_MY_CUSTOM_ASM_INVALID_ARGUMENT);

    #define ADD_TABS() fprintf(file, "%s", tabsBuffer)

    IntermidReprCommand command = elemPtr->command;
    ADD_TABS();
    const char* strRepr = NULL;
    INTERMID_REPR_ERR_CHECK(getCommandStringRepr(command.commandType, &strRepr));

    #define ADD_COMMAND(format, argName) \
        fprintf(file, "%s" format, strRepr, command.argument.argName);

    switch (command.commandType) {
        case   PUSH_NUM_INTERMID_REPR_CMD: ADD_COMMAND(" %.3f\n", doubleNumber); break;
        case       CALL_INTERMID_REPR_CMD: ADD_COMMAND(" function_%s:\n",  functionName); break;
        case   PUSH_VAR_INTERMID_REPR_CMD: fprintf(file, "%s [BX + %d]\n", strRepr, -(command.argument.variableInd + 1)); break;
        case    POP_VAR_INTERMID_REPR_CMD: fprintf(file, "%s  [BX + %d]\n", strRepr, -(command.argument.variableInd + 1)); break;
        case UNCOND_JMP_INTERMID_REPR_CMD:
        case  IF_EQ_JMP_INTERMID_REPR_CMD:
            fprintf(file, "%s %s:\n", strRepr, getLabelNameById(funcRepr, command.argument.jumpDestLabelInd));
            break;
        case ENTER_INTERMID_REPR_CMD:
                        fprintf(file, "push BX\n");
            ADD_TABS(); fprintf(file, "push %d\n", funcRepr->numOfLocalVars);
            ADD_TABS(); fprintf(file, "add\n");
            ADD_TABS(); fprintf(file, "pop BX\n\n");
            break;
        case LEAVE_INTERMID_REPR_CMD:
            fprintf(file, "\n");
            ADD_TABS(); fprintf(file, "push BX\n");
            ADD_TABS(); fprintf(file, "push %d\n", funcRepr->numOfLocalVars);
            ADD_TABS(); fprintf(file, "sub\n");
            ADD_TABS(); fprintf(file, "pop BX\n");
            break;
        case IS_GREATER_OR_EQ_INTERMID_REPR_CMD: fprintf(file, "greaterOrEqualCmp\n"); break;
        case    IS_LESS_OR_EQ_INTERMID_REPR_CMD: fprintf(file, "lessOrEqualCmp\n");    break;
        case            IS_EQ_INTERMID_REPR_CMD: fprintf(file, "isEqualCmp\n");        break;
        case           RETURN_INTERMID_REPR_CMD: fprintf(file, "ret\n");               break;
        case            INPUT_INTERMID_REPR_CMD: fprintf(file, "in\n");                break;
        case           OUTPUT_INTERMID_REPR_CMD: fprintf(file, "out\n");               break;
        default:
            fprintf(file, "%s\n", strRepr);
            break;
    }

    return GEN_MY_CUSTOM_ASM_STATUS_OK;
}

static GenMyCustomAsmErrors addFunctionMyAsmCodeToFile(
    const FunctionRepr* funcRepr,
    FILE*               file
) {
    IF_ARG_NULL_RETURN(funcRepr);
    IF_ARG_NULL_RETURN(file);

    // create safety label to avoid going into function's code without it's call
    fprintf(file, "jmp %sSafetyLabel:\n", funcRepr->functionName);
    fprintf(file, "%s:\n",                funcRepr->functionName);

    const size_t MAX_DEPTH_IN_BLOCKS_OF_CODE             = 50;
    char         tabsBuffer[MAX_DEPTH_IN_BLOCKS_OF_CODE] = {};
    const IntermidReprElement* elemPtr = funcRepr->listHead;
    while (elemPtr != NULL) {
        // clear tabs buffer
        memset(tabsBuffer, 0, MAX_DEPTH_IN_BLOCKS_OF_CODE * sizeof(char));
        for (size_t i = 0; i < elemPtr->depthInBlocksOfCode; ++i)
            tabsBuffer[i] = '\t';

        if (elemPtr->isCommand) {
            IF_ERR_RETURN(addMyAsmCommandStrRepr2file(funcRepr, elemPtr, tabsBuffer, file));
        } else {
            fprintf(file, "%s%s:\n", tabsBuffer,
                getLabelNameById(funcRepr, elemPtr->label.labelInd));
        }

        elemPtr = elemPtr->nextElement;
    }

    fprintf(file, "%sSafetyLabel:\n\n", funcRepr->functionName);
    // TODO: main to const or variable
    if (strcmp(funcRepr->functionName, "main") == 0) {
        fprintf(file, "call main:\n");
    }

    return GEN_MY_CUSTOM_ASM_STATUS_OK;
}




//  ---------------------------     NASM        ------------------------





static GenMyCustomAsmErrors addNasmCommandStrRepr2file(
    const FunctionRepr*        funcRepr,
    const IntermidReprElement* elemPtr,
    const char*                tabsBuffer,
    FILE*                      file,
    bool                       isMain
) {
    IF_ARG_NULL_RETURN(funcRepr);
    IF_ARG_NULL_RETURN(elemPtr);
    IF_ARG_NULL_RETURN(tabsBuffer);
    IF_ARG_NULL_RETURN(file);
    IF_NOT_COND_RETURN(elemPtr->isCommand, GEN_MY_CUSTOM_ASM_INVALID_ARGUMENT);

    #define ADD_TABS() fprintf(file, "%s", tabsBuffer)

    IntermidReprCommand command = elemPtr->command;
    ADD_TABS();
    const char* strRepr = NULL;
    INTERMID_REPR_ERR_CHECK(getCommandStringRepr(command.commandType, &strRepr));

    // ASK: maybe there's a better solution to deal with empty argument
    #define ADD_LINE(format, ...)          fprintf(file,      format,             __VA_ARGS__)
    #define ADD_TAB_LINE(format, ...)      fprintf(file, "%s" format, tabsBuffer, __VA_ARGS__)
    #define ADD_LINE_IMPL(format)          fprintf(file,      format                         )
    #define ADD_TAB_LINE_IMPL(format, ...) fprintf(file, "%s" format, tabsBuffer             )
    #define ADD_COMMAND(format, argName) ADD_LINE("%s" format, strRepr, command.argument.argName);

    // takes 2 arguments from stack, moves to registers rax, rbx
    // then compares it using cmp instruction
    // after that by using setxxxx command we store boolean value (0 or 1) to the rcx
    // and at the end we push this value back to the stack
    // cmpCommandShortSuf \in {"l", "le", "g", "ge", "e"}
    #define COMMON_CMP_DEF_CODE(cmpCommandShortSuf)  \
            ADD_LINE_IMPL("xor  rcx, rcx\n"               );    \
        ADD_TAB_LINE_IMPL("pop  rbx\n"                    );    \
        ADD_TAB_LINE_IMPL("pop  rax\n"                    );    \
        ADD_TAB_LINE_IMPL("cmp  rax, rbx\n"               );    \
        ADD_TAB_LINE_IMPL("set" cmpCommandShortSuf " cl\n");    \
        ADD_TAB_LINE_IMPL("push rcx\n"                    );    \
            ADD_LINE_IMPL("\n");

    #define COMMON_BIN_OP_CODE(commandName)            \
            ADD_LINE_IMPL("pop  rbx\n");               \
        ADD_TAB_LINE_IMPL("pop  rax\n");               \
        ADD_TAB_LINE_IMPL(commandName " rax, rbx\n");  \
        ADD_TAB_LINE_IMPL("push rax\n");               \
        ADD_LINE_IMPL("\n");

    #define COMMON_MUL_OR_DIV_CODE(commandName)        \
        ADD_LINE_IMPL("pop rbx\n");                    \
        ADD_TAB_LINE_IMPL("pop rax\n");                \
        ADD_TAB_LINE_IMPL(commandName " rbx\n");       \
        ADD_TAB_LINE_IMPL("push rax\n");               \
        ADD_LINE_IMPL("\n");

    // for function arguments:
    // [rbp]      - contains saved rbp value
    // [rbp + 8]  - contains return address
    // [rbp + 16] - contains first argument
    // [rbp + 24] - contains second argument
    // .....
    #define GET_VAR_IND() \
        (command.argument.variableInd < funcRepr->numOfArguments             \
            ? +(command.argument.variableInd + 2)                            \
            : -(command.argument.variableInd - funcRepr->numOfArguments + 1) \
        ) * 8

    switch (command.commandType) {
        // how to work with double numbers?
        case PARAM_IN_NUM_INTERMID_REPR_CMD:
        case     PUSH_NUM_INTERMID_REPR_CMD: ADD_LINE("push %d\n", (int)command.argument.doubleNumber); break;
        case         CALL_INTERMID_REPR_CMD: ADD_COMMAND(" function_%s\n",   functionName); break;
        // we add function_ prefix so that we don't accidentally name our function as one of reserved asm keywords
        case PARAM_IN_VAR_INTERMID_REPR_CMD:
        case     PUSH_VAR_INTERMID_REPR_CMD: ADD_LINE(  "push qword [rbp + %d]\n",          GET_VAR_IND()); break;
        case      POP_VAR_INTERMID_REPR_CMD: ADD_LINE( "%s  qword [rbp + %d]\n",   strRepr, GET_VAR_IND()); break;
        case    PARAM_OUT_INTERMID_REPR_CMD: ADD_LINE_IMPL("\n"); break; // TODO:
        case    IF_EQ_JMP_INTERMID_REPR_CMD:
                ADD_LINE_IMPL("\n");
            ADD_TAB_LINE_IMPL("pop rbx\n");
            ADD_TAB_LINE_IMPL("pop rax\n");
            ADD_TAB_LINE_IMPL("cmp rax, rbx\n");
                 ADD_TAB_LINE("%s %s\n", strRepr, getLabelNameById(funcRepr, command.argument.jumpDestLabelInd));
                ADD_LINE_IMPL("\n");
            break;
        case   UNCOND_JMP_INTERMID_REPR_CMD:
            ADD_LINE("%s %s\n", strRepr, getLabelNameById(funcRepr, command.argument.jumpDestLabelInd));
            break;
        case ENTER_INTERMID_REPR_CMD:
            ADD_LINE("enter %d, 0\n", (funcRepr->numOfLocalVars - funcRepr->numOfArguments) * 8);
            break;
        case LEAVE_INTERMID_REPR_CMD:
                ADD_LINE_IMPL("pop  rax\n"); // save func res
            ADD_TAB_LINE_IMPL("leave\n");
            break;
        case ADD_INTERMID_REPR_CMD:              COMMON_BIN_OP_CODE("add ");    break;
        case SUB_INTERMID_REPR_CMD:              COMMON_BIN_OP_CODE("sub ");    break;
        case MUL_INTERMID_REPR_CMD:              COMMON_MUL_OR_DIV_CODE("mul"); break;
        case DIV_INTERMID_REPR_CMD:              COMMON_MUL_OR_DIV_CODE("div"); break;
        case IS_GREATER_OR_EQ_INTERMID_REPR_CMD: COMMON_CMP_DEF_CODE("ge");     break;
        case    IS_LESS_OR_EQ_INTERMID_REPR_CMD: COMMON_CMP_DEF_CODE("le");     break;
        case          IS_LESS_INTERMID_REPR_CMD: COMMON_CMP_DEF_CODE("l");      break;
        case       IS_GREATER_INTERMID_REPR_CMD: COMMON_CMP_DEF_CODE("g");      break;
        case            IS_EQ_INTERMID_REPR_CMD: COMMON_CMP_DEF_CODE("e");      break;
        case           OUTPUT_INTERMID_REPR_CMD:
            // int to be printed (argument) is already in stack
                ADD_LINE_IMPL("push singleIntFormatSpec\n");
            ADD_TAB_LINE_IMPL("call myPrintfFunction\n");
            break;
        case            INPUT_INTERMID_REPR_CMD:

            break;
        case           RETURN_INTERMID_REPR_CMD:
            if (isMain) {
                    ADD_LINE_IMPL("call clearAndOutputBuffer\n");
                ADD_TAB_LINE_IMPL("pop  rdi\n"); // what if for some reason exit code is not zero
                ADD_TAB_LINE_IMPL("mov  rax, 60\n");
                ADD_TAB_LINE_IMPL("syscall\n");
            } else {
                    ADD_LINE_IMPL("pop  rbx\n"); // save return address
                     ADD_TAB_LINE("add  rsp, %d\n", funcRepr->numOfArguments * 8);
                ADD_TAB_LINE_IMPL("push rax\n"); // we assume that function result is stored in rax
                ADD_TAB_LINE_IMPL("push rbx\n"); // restore return address
                ADD_TAB_LINE_IMPL("ret\n");
            }
            break;
        default:
            fprintf(file, "%s\n", strRepr);
            break;
    }

    return GEN_MY_CUSTOM_ASM_STATUS_OK;
}

static GenMyCustomAsmErrors addFunctionNasmCodeToFile(
    const FunctionRepr* funcRepr,
    FILE*               file
) {
    IF_ARG_NULL_RETURN(funcRepr);
    IF_ARG_NULL_RETURN(file);

    bool isMain = strcmp(funcRepr->functionName, "main") == 0;
    if (!isMain) {
        ADD_LINE("function_%s:\n", funcRepr->functionName);
    } else {
        ADD_LINE_IMPL("_start:\n");
    }

    const size_t MAX_DEPTH_IN_BLOCKS_OF_CODE             = 50;
    char         tabsBuffer[MAX_DEPTH_IN_BLOCKS_OF_CODE] = {};
    const IntermidReprElement* elemPtr = funcRepr->listHead;
    while (elemPtr != NULL) {
        // clear tabs buffer
        memset(tabsBuffer, 0, MAX_DEPTH_IN_BLOCKS_OF_CODE * sizeof(char));
        for (size_t i = 0; i < elemPtr->depthInBlocksOfCode; ++i)
            tabsBuffer[i] = '\t';

        if (elemPtr->isCommand) {
            IF_ERR_RETURN(addNasmCommandStrRepr2file(funcRepr, elemPtr, tabsBuffer, file, isMain));
        } else {
            fprintf(file, "%s%s:\n", tabsBuffer,
                getLabelNameById(funcRepr, elemPtr->label.labelInd));
        }

        elemPtr = elemPtr->nextElement;
    }

    return GEN_MY_CUSTOM_ASM_STATUS_OK;
}










GenMyCustomAsmErrors genMyCustomAsmAndSaveIt2File(
    const IntermidRepr* intermidRepr,
    const char*         destFilePath,
    bool                isMyAsm
) {
    IF_ARG_NULL_RETURN(intermidRepr);
    IF_ARG_NULL_RETURN(destFilePath);

    FILE* file = fopen(destFilePath, "w");
    IF_NOT_COND_RETURN(file != NULL, GEN_MY_CUSTOM_ASM_FILE_OPENING_ERROR);

    if (!isMyAsm) {
        ADD_LINE_IMPL(
            "section .data\n\n"
            "singleIntFormatSpec db \"%%d\", 10, 0\n\n"
            "section .text\n"
            "extern myPrintfFunction\n"
            "extern clearAndOutputBuffer\n"
            "global _start\n\n"
        );
    }

    const FunctionRepr* funcRepr = intermidRepr->funcReprListHead;
    while (funcRepr != NULL) {
        if (isMyAsm) {
            IF_ERR_RETURN(addFunctionMyAsmCodeToFile(funcRepr, file));
        } else {
            IF_ERR_RETURN( addFunctionNasmCodeToFile(funcRepr, file));
        }
        funcRepr = funcRepr->nextFunc;
    }

    fclose(file);

    return GEN_MY_CUSTOM_ASM_STATUS_OK;
}
