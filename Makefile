CC			 := g++
CFLAGS 		 := -D _DEBUG -lm -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla
LOGGER_FLAGS :=
# CFLAGS += -Wcast-qual

# CFLAGS = -D _DEBUG
# CFLAGS += -D NDEBUG
CFLAGS += -fsanitize=address

MY_LOG_LIB_NAME    				:= my_loglib

 LEXEM_ANALYSATOR_PATH			:= LexemAnalysator
SYNTAX_ANALYSATOR_PATH			:= SyntaxAnalysator
   CODE_GENERATOR_PATH			:= CodeGenerator
    INTERMID_REPR_PATH			:= IntermediateRepresentation

 LEXEM_ANALYSATOR_BUILD_NAME	:= LexemAnalysator
SYNTAX_ANALYSATOR_BUILD_NAME	:= SyntaxAnalysator
   CODE_GENERATOR_BUILD_NAME	:= CodeGenerator
    INTERMID_REPR_BUILD_NAME	:= IntermidRepr

PROCESSOR_SUBMODULE_PATH		:= external/SPUprocessorEmulator
				ASM_PATH		:= $(PROCESSOR_SUBMODULE_PATH)/assembler
			PROCESSOR_PATH		:= $(PROCESSOR_SUBMODULE_PATH)/processor

 LEXEM_ANALYSATOR_BUILD_PATH	:= $(LEXEM_ANALYSATOR_PATH)/building
SYNTAX_ANALYSATOR_BUILD_PATH	:= $(SYNTAX_ANALYSATOR_PATH)/building
	INTERMID_REPR_BUILD_PATH	:= $(INTERMID_REPR_PATH)/building
# CODE_GENERATOR_BUILD_PATH		:= $(CODE_GENERATOR_BUILD_PATH)/building


LIB_RUN_NAME       				:= customProgrammingLang
BUILD_DIR          				:= building

ifeq ($(DEBUG), 0)
	ASSERT_DEFINE = -DNDEBUG
endif

.PHONY: $(LIB_RUN_NAME) run $(BUILD_DIR) clean


# GET RID OF SANITIZER DEADLY SIGNAL:		sudo sysctl vm.mmap_rnd_bits=28


# -------------------------   LIB RUN   -----------------------------

LEXEM_ANALYSATOR_MODULE:
	@make -s -C $(LEXEM_ANALYSATOR_PATH) clean
	@make -s -C $(LEXEM_ANALYSATOR_PATH) run

SYNTAX_ANALYSATOR_MODULE:
	@make -s -C $(SYNTAX_ANALYSATOR_PATH) clean
	@make -s -C $(SYNTAX_ANALYSATOR_PATH) run

INTERMID_REPR_MODULE:
	@make -C $(INTERMID_REPR_PATH) clean
	make -C $(INTERMID_REPR_PATH) ASM_OPT_ARG=$(MY_ASM) run

# CODE_GENERATOR_MODULE:
# 	@make -C $(CODE_GENERATOR_PATH) clean
# 	@make -C $(CODE_GENERATOR_PATH) run

OBJ := \
	$(LEXEM_ANALYSATOR_BUILD_PATH)/%.o\
	$(SYNTAX_ANALYSATOR_BUILD_PATH)/%.o\
	$(INTERMID_REPR_BUILD_PATH)/%.o\
#$(CODE_GENERATOR_BUILD_PATH)/%.o\

CFLAGS += -I $(LOGGER_EXT_LIB_DIR)/include

$(LIB_RUN_NAME): LEXEM_ANALYSATOR_MODULE SYNTAX_ANALYSATOR_MODULE INTERMID_REPR_MODULE
#$(LIB_RUN_NAME): LEXEM_ANALYSATOR_MODULE SYNTAX_ANALYSATOR_MODULE
#@$(CC) $(OBJ) -o $(BUILD_DIR)/$(LIB_RUN_NAME) -l$(MY_LOG_LIB_NAME) $(CFLAGS)

# WARNING: always cleans building directory

# runProcessor:
# 	@make -C $(ASM_PATH) run
# 	@make -C $(PROCESSOR_PATH) run

genSourceCode: clean $(LIB_RUN_NAME)
# ./$(LEXEM_ANALYSATOR_BUILD_PATH)/$(LEXEM_ANALYSATOR_BUILD_NAME)
# ./$(SYNTAX_ANALYSATOR_BUILD_PATH)/$(SYNTAX_ANALYSATOR_BUILD_NAME)
# ./$(INTERMID_REPR_BUILD_PATH)/$(INTERMID_REPR_BUILD_NAME)

# TODO:
runNasm:
	@make -C $(INTERMID_REPR_PATH) runAndCompileNasm
	@$(INTERMID_REPR_BUILD_PATH)/nasmCodeProg

runMyAsm:
	@make -C $(INTERMID_REPR_PATH) runMyAsm

# -------------------------   HELPER TARGETS   ---------------------------

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
clean:
	@rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(LIB_RUN_NAME)
