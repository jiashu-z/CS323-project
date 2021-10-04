CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(CS323-project1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
include_directories(src)
include(CTest)
set(CMAKE_CXX_STANDARD 11)
set($ENV{CTEST_OUTPUT_ON_FAILURE} TRUE)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

find_package(BISON)
find_package(FLEX)
FLEX_TARGET(scanner src/lexer.l src/lexer.cpp)
BISON_TARGET(parser src/parser.y ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp
        COMPILE_FLAGS  --debug
        DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.h
        VERBOSE
        )

ADD_FLEX_BISON_DEPENDENCY(scanner parser)
add_library(lexer SHARED ${FLEX_scanner_OUTPUTS})




include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(main
        src/parserMain.cpp
        src/token.cpp
        src/SyntaxTreeNode.cpp
        src/parser.cpp
        )
target_link_libraries(main
PUBLIC fl)



add_executable(CS323-project1
        src/parser.cpp
src/token.cpp
src/main.cpp
        src/SyntaxTreeNode.h
        src/SyntaxTreeNode.cpp
        )
target_link_libraries(CS323-project1 PUBLIC fl)




add_executable(test_lexer demo_test/test_lexer.cpp)
target_include_directories(test_lexer
PUBLIC src)

add_test(test_lexer test_lexer)

add_executable(test_token
        demo_test/test_token.cpp
src/token.cpp)
target_include_directories(test_token
PUBLIC src)

add_test(test_token test_token)

add_executable(test_syntaxTree demo_test/test_syntaxTree.cpp
        src/SyntaxTreeNode.cpp
        )
target_include_directories(test_syntaxTree
        PUBLIC src)
add_test(test_tree test_syntaxTree)


add_executable(testInitList src/testInitializeList.cpp)
