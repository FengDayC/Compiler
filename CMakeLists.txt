# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Compiler")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#public Variables
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PCH_DIR ${PROJECT_DIR}/pch.h)
set(APP_DIR ${PROJECT_DIR}/Compiler)
set(LEX_DIR ${PROJECT_DIR}/LexicalAnalyzer)
set(PARSE_DIR ${PROJECT_DIR}/Parser)
set(APP_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/Complier)
set(LEX_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/LexicalAnalyzer)
set(PARSE_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/Parser)

# Include sub-projects.
add_subdirectory ("Compiler")
add_subdirectory ("LexicalAnalyzer")
add_subdirectory ("Parser")
