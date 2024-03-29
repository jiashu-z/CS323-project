GPP=g++
FLEX=flex
BISON=bison
CXX_STD = -std=c++17

CXX_DEBUG = -g -Wall -Wextra
SRC_PATH=src
TEST_FILE_PATH=project2_test
CXX_FLAGS = -O3 $(CXX_DEBUG) $(CXX_STD)
CPP = $(GPP) $(CXX_FLAGS)

.PHONY: clean
.lex:$(SRC_PATH)/lexer.l
	$(FLEX) --outfile=$(SRC_PATH)/lexer.cpp $(SRC_PATH)/lexer.l
.syntax:$(SRC_PATH)/parser.y
	$(BISON) -t -v --defines=$(SRC_PATH)/parser.h --output=$(SRC_PATH)/parser.cpp $(SRC_PATH)/parser.y
splc: .lex  .syntax
	mkdir -p bin
	$(CPP) $(SRC_PATH)/parserMain.cpp \
    $(SRC_PATH)/SyntaxTreeNode.cpp \
    $(SRC_PATH)/parser.cpp \
    $(SRC_PATH)/SymbolTable.cpp \
    $(SRC_PATH)/SemanticAnalyzer.cpp \
    $(SRC_PATH)/IntermediateCodeTranlator.cpp\
    $(SRC_PATH)/IntermediateCode.cpp\
    $(SRC_PATH)/ir_optimizer.cpp\
    -L$(SRC_PATH) -lfl -ly \
    -o bin/splc
clean:
	@rm -rf $(SRC_PATH)/lexer.cpp \
	$(SRC_PATH)/parser.cpp \
	$(SRC_PATH)/parser.h \
	$(SRC_PATH)/parser.output \
	$(SRC_PATH)/*.o \
	$(SRC_PATH)/*.a \
	$(SRC_PATH)/*.so \
	$(TEST_FILE_PATH)/*/*.test.out \
	bin/splc
