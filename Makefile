GPP=g++
FLEX=flex
BISON=bison
CXX_STD = -std=c++17

CXX_DEBUG = -g -Wall -Wextra
SRC_PATH=src
CXX_FLAGS = -O3 $(CXX_DEBUG) $(CXX_STD)
CPP = $(GPP) $(CXX_FLAGS)

.PHONY: clean
.lex:$(SRC_PATH)/lexer.l
	$(FLEX) --outfile=$(SRC_PATH)/lexer.cpp $(SRC_PATH)/lexer.l
.syntax:$(SRC_PATH)/parser.y
	$(BISON) -t -v --defines=$(SRC_PATH)/parser.hpp --output=$(SRC_PATH)/parser.cpp $(SRC_PATH)/parser.y
splc: .lex  .syntax
	$(CPP) $(SRC_PATH)/parserMain.cc \
    $(SRC_PATH)/token.cpp \
    $(SRC_PATH)/SyntaxTreeNode.cc \
    $(SRC_PATH)/parser.cpp \
    -L$(SRC_PATH) -lfl -ly \
    -o bin/splc
clean:
	@rm -rf $(SRC_PATH)/lexer.cpp \
	$(SRC_PATH)/parser.cpp \
	$(SRC_PATH)/parser.hpp \
	$(SRC_PATH)/parser.output \
	$(SRC_PATH)/*.o \
	$(SRC_PATH)/*.a \
	$(SRC_PATH)/*.so \
	bin/splc