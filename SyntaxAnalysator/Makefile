CC			 := g++
CFLAGS 		 := -D _DEBUG -lm -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla
LOGGER_FLAGS :=
# CFLAGS += -Wcast-qual
# CFLAGS = -D _DEBUG
# CFLAGS += -D NDEBUG
CFLAGS += -fsanitize=address

MY_LOG_LIB_NAME    			:= my_loglib
SOURCE_DIR		   			:= source
LEXEM_REALIZATIONS_PATH		:= ../LexemsRealizations/source
SYNTAX_TREE_PATH			:= ../SyntaxTree/source
DUMPER_MODULE_PATH 			:= ../Dumper/source
MODULE_NAME       			:= SyntaxAnalysator
BUILD_DIR          			:= building
LOGGER_EXT_LIB_DIR 			:= external/LoggerLib

ifeq ($(DEBUG), 0)
	ASSERT_DEFINE = -DNDEBUG
endif

.PHONY: $(MODULE_NAME) run $(BUILD_DIR) clean

# -------------------------   LIB RUN   -----------------------------

# SRC 	   		   := $(SOURCE_DIR)/decisionTreeErrors.cpp $(SOURCE_DIR)/decisionTreeLib.cpp $(SOURCE_DIR)/dumper.cpp $(SOURCE_DIR)/main.cpp
SRC := 						\
	$(LEXEM_REALIZATIONS_PATH)/lexemsRealizations.cpp\
	$(LEXEM_REALIZATIONS_PATH)/errorsHandler.cpp\
	$(SYNTAX_TREE_PATH)/syntaxTree.cpp\
	$(SYNTAX_TREE_PATH)/syntaxTreeErrorsHandler.cpp\
	$(SYNTAX_TREE_PATH)/assignParents.cpp\
	$(DUMPER_MODULE_PATH)/dumper.cpp\
	$(SOURCE_DIR)/main.cpp					\
	$(SOURCE_DIR)/syntaxAnalysator.cpp					\
	$(SOURCE_DIR)/syntaxAnalysatorErrorsHandler.cpp					\

OBJ := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(notdir ${SRC}))

CFLAGS += -I $(LOGGER_EXT_LIB_DIR)/include

$(MODULE_NAME): $(OBJ)
	@$(CC) -w $^ -o $(BUILD_DIR)/$(MODULE_NAME) -l$(MY_LOG_LIB_NAME) $(CFLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp | $(BUILD_DIR)
	@$(CC) -w -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE)

$(BUILD_DIR)/%.o: $(LEXEM_REALIZATIONS_PATH)/%.cpp | $(BUILD_DIR)
	@$(CC) -w -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE)

$(BUILD_DIR)/%.o: $(SYNTAX_TREE_PATH)/%.cpp | $(BUILD_DIR)
	@$(CC) -w -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE)

$(BUILD_DIR)/%.o: $(DUMPER_MODULE_PATH)/%.cpp | $(BUILD_DIR)
	@$(CC) -w -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE)

# WARNING: always cleans building directory

run: clean $(MODULE_NAME)
	@./building/$(MODULE_NAME)

# -------------------------   HELPER TARGETS   ---------------------------

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
clean:
	@rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(MODULE_NAME)
