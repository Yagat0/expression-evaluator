#include "expression_evaluator.hpp"

int main() {
    std::string expression;

    std::cout << "Enter an expression: ";
    std::getline(std::cin, expression);

    std::cout << evaluate(expression);
}