#include "expression_evaluator.hpp"

// checks whether op1 has lower precedence compared to op2 or is left associative
bool has_lower_precedence(const std::string& op1_str, const std::string& op2_str) {
    const OperatorProperty& op1 = operator_properties.at(op1_str);
    const OperatorProperty& op2 = operator_properties.at(op2_str);
    if(op1.priority == op2.priority) {
        return op1.left_associative;
    }
    return op1.priority < op2.priority;
}

// converts expression from infix to postfix notation using shunting yard algorithm
// https://en.wikipedia.org/wiki/Shunting_yard_algorithm
std::stack<std::string> infix_to_postfix(const std::string& expression) {
    std::stack<std::string> output;
    std::stack<std::string> Operators;

    std::string current_num;
    bool previous_num = false; // tracks if last character was a number

    for (std::size_t i = 0; i < expression.length(); ++i) {
        const char current_char = expression[i];

        if (isdigit(current_char) || current_char == '.' || current_char == ',') { // parse number
            if(current_char == ',') { // is decimal point
                current_num += '.';
            } else { // is number
                current_num += current_char;
            }
            previous_num = true;
        } else if (current_char != ' ') { // character is arithmetic or unary operator
            if(i - 1 < 0 || !isdigit(expression[i-1])) { // unary operator
                current_num+=current_char;
                continue;
            }

            // push parsed number to output stack
            if (previous_num) {
                output.push(current_num);
                current_num.clear();
                previous_num = false;
            }

            // handle operators
            const std::string op (1, current_char);
            while (!Operators.empty() && has_lower_precedence(op, Operators.top())) {
                output.push(Operators.top());
                Operators.pop();
            }
            Operators.push(op);
        }
    }

    // release last number if present
    if (!current_num.empty()) {
        output.push(current_num);
    }

    // push the rest of operators
    while (!Operators.empty()) {
        output.push(Operators.top());
        Operators.pop();
    }
    reverse_stack(output); // reverse the output stack to get correct order
    return output;
}

// checks whether string is a number
bool is_number(const std::string& s)
{
    return s.find_first_of("0123456789.") != std::string::npos;
}

// converts arithmetic operator to an Operator enum
Operator Operator_to_enum(const std::string& op) {
    if(op == "+")
        return Operator::Addition;
    if(op == "-")
        return Operator::Subtraction;
    if(op == "*")
        return Operator::Multiplication;
    if(op == "/")
        return Operator::Division;
    if(op == "^")
        return Operator::Exponentiation;
    throw std::runtime_error("Invalid operator: " + op);
}

// applies an Operator on the first two top operands from the result stack
void apply_Operator(std::stack<double>& result, Operator op) {
    if (result.size() < 2) {
        throw std::runtime_error("Not enough operands to perform operation");
    }

    double num1, num2;
    
    num1 = result.top();
    result.pop();
    num2 = result.top();
    result.pop();

    switch(op) {
        case Operator::Addition:
            result.push(num2+num1);
            break;
        case Operator::Subtraction:
            result.push(num2-num1);
            break;
        case Operator::Multiplication:
            result.push(num2*num1);
            break;
        case Operator::Division:
            result.push(num2/num1);
            break;
        case Operator::Exponentiation:
            result.push(std::pow(num2, num1));
            break;
        default:
            throw std::runtime_error("Invalid Operator enum value");
    }
}

// converts an expression to postfix notation and then evaluates it
double evaluate(const std::string& expression) {
    std::stack<std::string> postfix_expr = infix_to_postfix(expression);
    std::stack<double> result;

    if (postfix_expr.empty()) {
        throw std::runtime_error("Empty expression");
    }

    while(!postfix_expr.empty()) {
        const std::string& token = postfix_expr.top();

        if(is_number(token)){
            result.push(std::stod(token));
        } else {
            if (result.size() < 2) {
                throw std::runtime_error("Invalid expression: not enough operands");
            }

            Operator op;
               op = Operator_to_enum(token);
            apply_Operator(result, op);
        }
        postfix_expr.pop();
    }

    if (result.size() != 1) {
        throw std::runtime_error("Invalid expression: too many operands");
    }

    return result.top();
}