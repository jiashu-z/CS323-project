CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(CS323-project1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
include_directories(src)
include(CTest)
set($ENV{CTEST_OUTPUT_ON_FAILURE} TRUE)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ldl")

find_package(BISON)
find_package(FLEX)

message("maozunyao:" ${CMAKE_CURRENT_SOURCE_DIR})
FLEX_TARGET(scanner src/lexer.l ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.cpp)

BISON_TARGET(parser src/parser.y ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp
        DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.hpp)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

add_library(lexer SHARED ${FLEX_scanner_OUTPUTS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
message("maozunyao:${FLEX_parser_OUTPUTS},${BISON_scanner_OUTPUTS}")
add_executable(main
        src/parserMain.cc
        src/token.cpp
        src/SyntaxTreeNode.cc
#        ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp
        )
target_link_libraries(main
PUBLIC fl)
#target_link_libraries(main ${FLEX_LIBRARIES})

target_link_libraries(lexer
        PUBLIC fl
        )


#
#add_executable(CS323-project1
#src/token.cpp
#src/main.cpp
#        src/SyntaxTreeNode.hh
#        src/SyntaxTreeNode.cc)
#target_link_libraries(CS323-project1 PUBLIC fl)




#add_executable(test_lexer test/test_lexer.cpp)
#target_include_directories(test_lexer
#PUBLIC src)

#add_test(test_lexer test_lexer)

add_executable(test_token
test/test_token.cpp
src/token.cpp)
target_include_directories(test_token
PUBLIC src)

add_test(test_token test_token)

add_executable(test_tree test/test_syntaxTree.cpp
        src/SyntaxTreeNode.cc
        )
target_include_directories(test_tree
        PUBLIC src)
add_test(test_tree test_syntaxTree)


add_executable(testInitList src/testInitializeList.cc)
