CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(CS323-project1 C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

enable_testing()

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

find_package(BISON)
find_package(FLEX)

FLEX_TARGET(scanner src/lexer.l ${CMAKE_SOURCE_DIR}/src/lexer.cpp)
BISON_TARGET(parser src/parser.y ${CMAKE_SOURCE_DIR}/src/parser.cpp)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

add_library(lexer SHARED ${FLEX_scanner_OUTPUTS})

add_executable(CS323-project1 
src/token.cpp
src/main.cpp)

target_link_libraries(lexer
PUBLIC fl
)

target_link_libraries(CS323-project1
PUBLIC lexer
)

add_executable(test_lexer test/text_lexer.cpp)
target_include_directories(test_lexer
PUBLIC src)

add_test(test_lexer test_lexer)

# add_executable(parser
# ${BISON_parser_OUTPUTS})

# target_link_libraries(parser
# PUBLIC fl)
