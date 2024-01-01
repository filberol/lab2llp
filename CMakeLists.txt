cmake_minimum_required(VERSION 3.21)
project(lab2llp C)

set(CMAKE_C_STANDARD 11)

set(SRC_DIR "src")
set(INCLUDE_DIR "include")
set(TOKENS_DIR "grammar")
set(PARSER_DIR "generated")

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**.c)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/**.h)

find_program(FLEX_EXECUTABLE flex)
find_program(BISON_EXECUTABLE bison)

set(LEXER_OUT "${PARSER_DIR}/tokens.c")
set(PARSER_OUT "${PARSER_DIR}/grammar.c")

execute_process(
        COMMAND ${FLEX_EXECUTABLE} -o ${LEXER_OUT} ${TOKENS_DIR}/tokens.l
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
execute_process(
        COMMAND ${BISON_EXECUTABLE}   --graph  --report=itemset -d -o ${PARSER_OUT} ${TOKENS_DIR}/grammar.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(lab2 "${SRC_DIR}/main.c" ${LEXER_OUT} ${PARSER_OUT} ${SOURCES})

target_include_directories(lab2 PRIVATE ${INCLUDE_DIR})


