cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)

project(diagnosis)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(liblua)
add_subdirectory(circuit2cnf)
