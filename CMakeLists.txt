CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(CS323-project1 C CXX)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

find_package(BISON)
find_package(FLEX)

FLEX_TARGET(scanner src/lex.l ${CMAKE_SOURCE_DIR}/src/lexer.c)
BISON_TARGET(parser src/parser.y ${CMAKE_SOURCE_DIR}/src/parser.c)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

add_executable(CS323-project1 
src/main.cpp)

add_executable(lexer
${FLEX_scanner_OUTPUTS})

target_link_libraries(lexer
PUBLIC fl
)

# add_executable(parser
# ${BISON_parser_OUTPUTS})

# target_link_libraries(parser
# PUBLIC fl)
