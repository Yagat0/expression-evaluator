#ifndef EXPRESSION_EVALUATOR_HPP
#define EXPRESSION_EVALUATOR_HPP

#include <iostream>
#include <string>

#include <stack>
#include <unordered_map>
#include <queue>

#include <algorithm>
#include <cmath>

struct OperatorProperty {
    int priority;
    bool left_associative;
};

enum class Operator {
    Addition,
    Subtraction,
    Multiplication,
    Division,
    Exponentiation,
};

// priorities and left associativity of arithmetic operators
const std::unordered_map<std::string, OperatorProperty> operator_properties {
        {"+", {1, true}},
        {"-", {1, true}},
        {"*", {2, true}},
        {"/", {2, true}},
        {"^", {3, false}},
};

// outputs the elements of a stack to an ostream
template<typename T>
std::ostream& operator<<(std::ostream& os, std::stack<T> my_stack)
{
    while(!my_stack.empty()) //body
    {
        os << my_stack.top() << " ";
        my_stack.pop();
    }
    return os; // end of function
}

// checks whether op1 has lower precedence or is left associative
bool has_lower_precedence(const std::string& current_operation, const std::string& last_stack_operation);

// converts expression from infix to postfix notation
std::queue<std::string> infix_to_postfix(const std::string& expression);

// checks whether string is a number
bool is_number(const std::string& s);

// converts arithmetic operator to an Operator enum
Operator operator_to_enum(const std::string& op);

// applies an operation on the first two top operands from the result stack
void apply_operator(std::stack<double>& result, Operator op);

// converts an expression to postfix notation and then evaluates it
double evaluate(const std::string& expression);

#endif //EXPRESSION_EVALUATOR_HPP
