cmake_minimum_required(VERSION 3.12)
project(CS460Proj2)

set(CMAKE_CXX_STANDARD 14)

add_executable(CS460Proj2 LexicalAnalyzer.h LexicalAnalyzer.o Project2.cpp
                SetLimits.cpp SetLimits.h SyntacticalAnalyzer.cpp
                SyntacticalAnalyzer.h)

set_target_properties(CS460Proj2 PROPERTIES LINKER_LANGUAGE CXX)