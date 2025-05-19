CC			 := g++
CFLAGS 		 := -D _DEBUG -lm -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla
LOGGER_FLAGS :=
# CFLAGS = -D _DEBUG
# CFLAGS += -D NDEBUG
CFLAGS += -fsanitize=address

ASM_COMPILER 	  := nasm
ASM_COMPILER_ARGS := -f elf64

MY_LOG_LIB_NAME    			:= my_loglib
SOURCE_DIR		   			:= source
GEN_MY_ASM_CODE				:= ../GenAsmForMyProcessor/source
LEXEM_REALIZATIONS_PATH		:= ../LexemsRealizations/source
SYNTAX_TREE_PATH			:= ../SyntaxTree/source
DUMPER_MODULE_PATH 			:= ../Dumper/source
SEMANTIC_CHECK_MODULE_PATH  := ../SemanticChecker/source
MODULE_NAME       			:= IntermidRepr
BUILD_DIR_PATH          	:= building
BUILD_DIR_TARGET_NAME		:= createBuildDir
LOGGER_EXT_LIB_DIR 			:= external/LoggerLib

ASM_LIBS_FOLDER				:= asmLibs
NASM_SOURCE_FILE			:= asmDestFile.asm
PRINTF_LIB_SOURCE_FILE		:= $(ASM_LIBS_FOLDER)/printfAsmLib.asm
 SCANF_LIB_SOURCE_FILE		:= $(ASM_LIBS_FOLDER)/scanfAsmLib.asm
  HALT_LIB_SOURCE_FILE		:= $(ASM_LIBS_FOLDER)/haltAsmLib.asm

PRINTF_LIB_NAME				:= printfLib
 SCANF_LIB_NAME				:= scanfLib
  HALT_LIB_NAME				:= haltLib

NASM_EXEC_FILE_NAME			:= nasmCodeProg
PRINTF_LIB_EXEC_FILE_NAME	:= printfLib.o
 SCANF_LIB_EXEC_FILE_NAME   := scanfLib.o
  HALT_LIB_EXEC_FILE_NAME	:= haltLib.o

ifeq ($(DEBUG), 0)
	ASSERT_DEFINE = -DNDEBUG
endif

.PHONY: $(MODULE_NAME) run $(BUILD_DIR) clean

# -------------------------   LIB RUN   -----------------------------

SRC := 												 				\
	$(LEXEM_REALIZATIONS_PATH)/lexemsRealizations.cpp				\
	$(LEXEM_REALIZATIONS_PATH)/errorsHandler.cpp\
	$(SYNTAX_TREE_PATH)/syntaxTree.cpp\
	$(SYNTAX_TREE_PATH)/assignParents.cpp\
	$(SYNTAX_TREE_PATH)/syntaxTreeErrorsHandler.cpp\
	$(SEMANTIC_CHECK_MODULE_PATH)/semanticChecker.cpp\
	$(SEMANTIC_CHECK_MODULE_PATH)/semanticCheckerErrorsHandler.cpp\
	$(DUMPER_MODULE_PATH)/dumper.cpp\
	$(SOURCE_DIR)/main.cpp					\
	$(SOURCE_DIR)/intermidRepr.cpp					\
	$(SOURCE_DIR)/intermidReprErrorsHandler.cpp					\
	$(GEN_MY_ASM_CODE)/genMyCustomAsm.cpp						\
	$(GEN_MY_ASM_CODE)/genMyCustomAsmErrorsHandler.cpp			\


OBJ := $(patsubst %.cpp, $(BUILD_DIR_PATH)/%.o, $(notdir ${SRC}))

CFLAGS += -I $(LOGGER_EXT_LIB_DIR)/include

$(MODULE_NAME): $(OBJ)
	@$(CC) $^ -o $(BUILD_DIR_PATH)/$(MODULE_NAME) -l$(MY_LOG_LIB_NAME) $(CFLAGS)

$(BUILD_DIR_PATH)/%.o: $(SOURCE_DIR)/%.cpp | $(BUILD_DIR)
	@$(CC) -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE)

$(BUILD_DIR_PATH)/%.o: $(GEN_MY_ASM_CODE)/%.cpp | $(BUILD_DIR_TARGET_NAME)
	@$(CC) -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE)

$(BUILD_DIR_PATH)/%.o: $(LEXEM_REALIZATIONS_PATH)/%.cpp | $(BUILD_DIR_TARGET_NAME)
	@$(CC) -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE)

$(BUILD_DIR_PATH)/%.o: $(SYNTAX_TREE_PATH)/%.cpp | $(BUILD_DIR_TARGET_NAME)
	@$(CC) -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE)

$(BUILD_DIR_PATH)/%.o: $(DUMPER_MODULE_PATH)/%.cpp | $(BUILD_DIR_TARGET_NAME)
	@$(CC) -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE)

$(BUILD_DIR_PATH)/%.o: $(SEMANTIC_CHECK_MODULE_PATH)/%.cpp | $(BUILD_DIR_TARGET_NAME)
	@$(CC) -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE)

# to get rid of DEADLY SIGNAL: sudo sysctl vm.mmap_rnd_bits=28
# WARNING: always cleans building directory
run: clean $(MODULE_NAME)
	@$(BUILD_DIR_PATH)/$(MODULE_NAME)

runAndCompileNasm:
	@$(ASM_COMPILER) -o $(BUILD_DIR_PATH)/$(NASM_EXEC_FILE_NAME).o 	   $(NASM_SOURCE_FILE) 		 $(ASM_COMPILER_ARGS)
	@$(ASM_COMPILER) -o $(BUILD_DIR_PATH)/$(PRINTF_LIB_EXEC_FILE_NAME) $(PRINTF_LIB_SOURCE_FILE) $(ASM_COMPILER_ARGS)
	@$(ASM_COMPILER) -o $(BUILD_DIR_PATH)/$(SCANF_LIB_EXEC_FILE_NAME)   $(SCANF_LIB_SOURCE_FILE) $(ASM_COMPILER_ARGS)
	@$(ASM_COMPILER) -o $(BUILD_DIR_PATH)/$(HALT_LIB_EXEC_FILE_NAME)     $(HALT_LIB_SOURCE_FILE) $(ASM_COMPILER_ARGS)
	@ld -no-pie      -o $(BUILD_DIR_PATH)/$(NASM_EXEC_FILE_NAME) 	\
		$(BUILD_DIR_PATH)/$(NASM_EXEC_FILE_NAME).o 					\
		$(BUILD_DIR_PATH)/$(PRINTF_LIB_EXEC_FILE_NAME)				\
		$(BUILD_DIR_PATH)/$(SCANF_LIB_EXEC_FILE_NAME)				\
		$(BUILD_DIR_PATH)/$(HALT_LIB_EXEC_FILE_NAME)				\

# -------------------------   HELPER TARGETS   ---------------------------

$(BUILD_DIR_TARGET_NAME):
	@mkdir -p $(BUILD_DIR_PATH)
clean:
	@rm -rf $(BUILD_DIR_PATH)/*.o $(BUILD_DIR_PATH)/$(MODULE_NAME)
