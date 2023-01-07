#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <limits>

std::vector<std::string> get_file_content(const std::string &filepath);

class Operations {
public:
    Operations(std::string left = "", std::string operation = "", std::string right = "") : _left(left), _operation(operation), _right(right), _value(0), _is_evaluated(false) {}
    Operations(int64_t value) : _is_evaluated(true), _value(value) {}
    std::string _left;
    std::string _operation;
    std::string _right;
    int64_t _value;
    bool _is_evaluated;
};

std::vector<std::string> split(std::string string, const std::string &separator)
{
    std::vector<std::string> array;
    size_t index = 0;
    while ((index = string.find(separator)) != std::string::npos) {
        array.push_back(string.substr(0, index));
        string.erase(string.begin(), string.begin() + index + separator.length());
    }
    array.push_back(string);
    return (array);
}

int64_t evaluate_operation(Operations &operation, std::map<std::string,Operations> &operations)
{
    if (operation._is_evaluated)
        return operation._value;
    int64_t first = evaluate_operation(operations[operation._left], operations);
    int64_t second = evaluate_operation(operations[operation._right], operations);
    if (operation._operation == "+")
        operation._value = first + second;
    if (operation._operation == "-")
        operation._value = first - second;
    if (operation._operation == "*")
        operation._value = first * second;
    if (operation._operation == "/")
        operation._value = first / second;
    operation._is_evaluated = true;
    return operation._value;
}

int first(std::map<std::string,Operations> operations)
{
    evaluate_operation(operations.at("root"), operations);
    std::cout << operations.at("root")._value << std::endl;
}


// TO TRANSFORM INTO BINARY SEARCH
int second(std::map<std::string,Operations> operations)
{
    Operations root = operations.at("root");
    std::map<std::string,Operations> copy = operations;
    int64_t humn = 3876027196100;
    int64_t left = evaluate_operation(copy[root._left], copy);
    int64_t right = evaluate_operation(copy[root._right], copy);

    for ( ; left > right; humn += 1) {
        std::cout << "Values for humn = " << humn << std::endl;
        copy["humn"] = humn;
        left = evaluate_operation(copy[root._left], copy);
        std::cout << "Values for left = " << left << std::endl;

        std::cout << "Values for right = " << right << std::endl << std::endl;
        copy = operations;
    }
    std::cout << humn - 1 << std::endl;
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_21/input");
    std::map<std::string,Operations> operations;

    for (const auto &line: file_content) {
        std::vector <std::string> splitted_line = split(line, ": ");
        std::vector <std::string> splitted_operation = split(splitted_line[1], " ");
        if (splitted_operation.size() == 3)
            operations[splitted_line[0]] = Operations(splitted_operation[0], splitted_operation[1],splitted_operation[2]);
        else
            operations[splitted_line[0]] = Operations(std::stol(splitted_operation[0]));
    }
    second(operations);
    return (0);
}
