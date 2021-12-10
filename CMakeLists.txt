CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(CS323-project1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
include_directories(src)
include(CTest)
set(CMAKE_CXX_STANDARD 17)
set($ENV{CTEST_OUTPUT_ON_FAILURE} TRUE)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)



find_package(FLEX  REQUIRED)
SET(FlexOutput ${CMAKE_SOURCE_DIR}/src/lexer.cpp)
IF(FLEX_FOUND)
    ADD_CUSTOM_TARGET(lexer
            BYPRODUCTS ${FlexOutput}
            COMMAND ${FLEX_EXECUTABLE}
            --outfile=${FlexOutput}
            ${CMAKE_SOURCE_DIR}/src/lexer.l
            COMMENT "Building lexer.cpp"
    )
ENDIF()

find_package(BISON REQUIRED)
SET(BisonOutput ${CMAKE_SOURCE_DIR}/src/parser.cpp)
IF(BISON_FOUND)
    ADD_CUSTOM_TARGET(parser
            BYPRODUCTS ${BisonOutput} ${CMAKE_SOURCE_DIR}/src/parser.h ${CMAKE_SOURCE_DIR}/src/parser.output
            COMMAND ${BISON_EXECUTABLE}
            -t -v
            --defines=${CMAKE_SOURCE_DIR}/src/parser.h
            --output=${BisonOutput}
            ${CMAKE_SOURCE_DIR}/src/parser.y
            COMMENT "Building parser.cpp"
    )
ENDIF()



include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(main
        src/parserMain.cpp
        src/SyntaxTreeNode.cpp
        ${BisonOutput}
        src/SymbolTable.cpp
        src/SemanticAnalyzer.cpp
        src/IntermediateCode.cpp
        src/IntermediateCodeTranlator.cpp
        src/ir_optimizer.cpp
        )
add_dependencies(main lexer parser)
target_link_libraries(main
PUBLIC fl)


add_executable(test_syntaxTree
        test_executable/test_syntaxTree.cpp
  src/SyntaxTreeNode.cpp
  src/SymbolTable.cpp)

target_include_directories(test_syntaxTree
  PUBLIC src)
add_test(test_tree test_syntaxTree)

add_executable(test_symbolTable
        test_executable/test_symbolTable.cpp
        src/SymbolTable.cpp)

target_include_directories(test_syntaxTree
        PUBLIC src)
add_test(test_symbol test_symbolTable)

add_executable(testInitList src/testInitializeList.cpp)
