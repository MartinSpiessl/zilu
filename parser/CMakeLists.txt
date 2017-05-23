#cmake_minimum_required(VERSION 2.8)
find_package(FLEX)
find_package(BISON)

FLEX_TARGET(Lexer lex.l  ${CMAKE_CURRENT_BINARY_DIR}/lex.cpp)
BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

#include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(.)

file(GLOB PARSER_SRC "main.cpp")
#aux_source_directory(PARSER_SRC .)

#add_library(parser ${FLEX_MistralLexer_OUTPUTS}  ${BISON_MistralParser_OUTPUTS} ${PARSER_SRC})

add_executable(parser ${PARSER_SRC}
  ${FLEX_Lexer_OUTPUTS}
  ${BISON_Parser_OUTPUTS}
)
