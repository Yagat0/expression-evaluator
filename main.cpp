#include "expression_evaluator.hpp"
#include <format>

int main() {
    std::string expression;

    std::cout << "Enter an expression: ";
    std::getline(std::cin, expression);

    std::cout << std::format("{}", evaluate(expression));
}