#ifndef GEN_ASM_FOR_MY_PROCESSOR_GEN_MY_CUSTOM_ASM_HPP
#define GEN_ASM_FOR_MY_PROCESSOR_GEN_MY_CUSTOM_ASM_HPP

#include "../../IntermediateRepresentation/include/intermidRepr.hpp"
#include "genMyCustomAsmErrorsHandler.hpp"

GenMyCustomAsmErrors genMyCustomAsmAndSaveIt2File(
    const IntermidRepr* intermidRepr,
    const char*         destFilePath,
    bool                isMyAsm
);

#endif
