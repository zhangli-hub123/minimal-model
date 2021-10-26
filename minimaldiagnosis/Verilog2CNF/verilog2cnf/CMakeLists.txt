cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)

project(verilog2cnf)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register")

BISON_TARGET(verilog_parser
        verilog.yy
        ${CMAKE_CURRENT_BINARY_DIR}/verilog_parser.tab.cc)
FLEX_TARGET(verilog_lexer
        verilog.l
        ${CMAKE_CURRENT_BINARY_DIR}/verilog_lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(verilog_lexer verilog_parser)

add_executable(circuit2cnf main.cpp verilog_driver.cpp  ${FLEX_verilog_lexer_OUTPUTS}
        ${BISON_verilog_parser_OUTPUTS} cnf_driver.cpp cnf_driver.h)

target_include_directories(circuit2cnf
        PRIVATE
        ../liblua
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${FLEX_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(circuit2cnf lua)
