cmake_minimum_required(VERSION 3.28)
project(expression_evaluator)

set(CMAKE_CXX_STANDARD 20)

add_executable(expresion_evaluator main.cpp
        expression_evaluator.cpp
        expression_evaluator.hpp)