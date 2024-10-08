#include "expression_evaluator.hpp"

// checks whether op1 has lower precedence compared to op2 or is left associative
bool has_lower_precedence(const std::string& op1_str, const std::string& op2_str) {
    if (op1_str == "(" || op2_str == "(" || op1_str == ")" || op2_str == ")") {
        return false;
    }
    const OperatorProperty& op1 = operator_properties.at(op1_str);
    const OperatorProperty& op2 = operator_properties.at(op2_str);
    if(op1.priority == op2.priority) {
        return op1.left_associative;
    }
    return op1.priority < op2.priority;
}

// converts an expression from infix to postfix notation using shunting yard algorithm
// https://en.wikipedia.org/wiki/Shunting_yard_algorithm
std::stack<std::string> infix_to_postfix(const std::string& expression) {
    std::stack<std::string> output;
    std::stack<std::string> Operators;

    std::string current_num;
    bool previous_num = false; // tracks if last character was a number, used for number parsing

    for (std::size_t i = 0; i < expression.length(); ++i) {
        const char current_char = expression[i];

        // parse number
        if (isdigit(current_char) || current_char == '.' || current_char == ',') {
            if(current_char == ',') { // is decimal point
                current_num += '.';
            } else { // is number
                current_num += current_char;
            }
            previous_num = true;

        // parenthesis support
        } else if (current_char == '(') {
            Operators.emplace("(");
        } else if (current_char == ')') {
            // push last number in parentheses
            if (!current_num.empty()) {
                output.push(current_num);
                current_num.clear();
            }

            // push operators to output until '(' is encountered
            while (!Operators.empty() && Operators.top() != "(") {
                output.push(Operators.top());
                Operators.pop();
            }

            if (!Operators.empty()) {
                Operators.pop(); // Remove '(' from the stack
            } else {
                throw std::runtime_error("Mismatched parentheses");
            }

        // character is an arithmetic or unary operator
        } else if (current_char != ' ') {
            if(i - 1 < 0 || !isdigit(expression[i-1])) { // unary operator
                current_num += current_char;
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
            while (!Operators.empty() && Operators.top() != "(" && has_lower_precedence(op, Operators.top())) {
                output.push(Operators.top());
                Operators.pop();
            }
            Operators.push(op);
        }
    }

    // push last parsed number to output if present
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
Operator operator_to_enum(const std::string& op) {
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
void apply_operator(std::stack<double>& result, Operator op) {
    if (result.size() < 2) {
        throw std::runtime_error("Not enough operands to perform operation");
    }
    
    double num1 = result.top();
    result.pop();
    double num2 = result.top();
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
            if(num1 == 0.0)
                throw std::runtime_error("Division by zero");
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
    if(expression.empty())
        throw std::runtime_error("Empty expression");

    std::stack<std::string> postfix_expr = infix_to_postfix(expression);
    std::stack<double> result;

    while(!postfix_expr.empty()) {
        const std::string& token = postfix_expr.top();

        if(is_number(token)){
            try {
                result.push(std::stod(token));
            } catch (const std::invalid_argument& e) {
                throw std::runtime_error("Invalid number: " + token + "\n");
            } catch (const std::out_of_range& e) {
                throw std::runtime_error("Number out of range: " + token + "\n");
            }
        } else {
            if (result.size() < 2) {
                throw std::runtime_error("Invalid expression: not enough operands");
            }

            Operator op;
               op = operator_to_enum(token);
            apply_operator(result, op);
        }
        postfix_expr.pop();
    }

    if (result.size() != 1) {
        throw std::runtime_error("Invalid expression: too many operands");
    }
    return result.top();
}