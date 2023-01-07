#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <deque>

std::vector<std::string> get_file_content(const std::string &filepath);

class Monkey {
public:
    Monkey(std::deque<uint64_t> starting_items_worry_level, bool is_operation_multiplication, bool operation_is_squared, uint64_t operation_value, uint64_t test_condition, uint64_t monkey_true, uint64_t monkey_false) : _starting_items_worry_level(std::move(starting_items_worry_level)), _is_operation_multiplication(is_operation_multiplication), _operation_is_squared(operation_is_squared), _operation_value(operation_value), _test_condition(test_condition), _monkey_true(monkey_true), _monkey_false(monkey_false) {
        _inspected = 0;
    }
    void apply_operation_on_first_item() {
        uint64_t second_value = _operation_is_squared ? _starting_items_worry_level[0] : _operation_value;
        _starting_items_worry_level[0] =  _is_operation_multiplication ? _starting_items_worry_level[0] * second_value : _starting_items_worry_level[0] + second_value;
    }
    void apply_relief_on_first_item() {
        _starting_items_worry_level[0] = _starting_items_worry_level[0] / 3;
    }
    uint64_t get_next_monkey_index() {
        return  _starting_items_worry_level[0] % _test_condition == 0 ? _monkey_true : _monkey_false;
    }
    std::deque<uint64_t> _starting_items_worry_level;
    bool _is_operation_multiplication;
    bool _operation_is_squared;
    uint64_t _operation_value;
    uint64_t _test_condition;
    uint64_t _monkey_true;
    uint64_t _monkey_false;
    uint64_t _inspected;
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

std::vector<Monkey> parse_file(std::vector<std::string> file_content)
{
    std::vector<Monkey> monkeys;
    for (size_t i = 1; i < file_content.size(); i++) {
        std::string starting_items_line = file_content[i++];
        std::string operation_line = file_content[i++];
        uint64_t test_condition = std::stoi(file_content[i].substr(file_content[i].find_last_of(' ') + 1, file_content[i].length() - file_content[i].find_last_of(' ') + 1));
        uint64_t monkey_true = std::stoi(file_content[++i].substr(file_content[i++].length() - 1, 1));
        uint64_t monkey_false = std::stoi(file_content[i].substr(file_content[i++].length() - 1, 1));
        std::deque<uint64_t> starting_items_worry_level;
        std::vector<std::string> starting_items_worry_level_string;
        starting_items_line = starting_items_line.erase(0, starting_items_line.find(": ") + 2);
        starting_items_worry_level_string = split(starting_items_line, ", ");
        std::transform(starting_items_worry_level_string.begin(), starting_items_worry_level_string.end(), std::back_inserter(starting_items_worry_level), [](const std::string &str) {return std::stoul(str);});
        std::vector<std::string> operation_array = split(operation_line, " ");
        bool is_operation_multiplication = operation_array[6] == "*";
        bool operation_squared = operation_array[7] == "old";
        uint64_t operation_value = operation_squared ? 0 : std::stoi(operation_array[7]);
        Monkey monkey(starting_items_worry_level, is_operation_multiplication, operation_squared, operation_value, test_condition, monkey_true, monkey_false);
        monkeys.push_back(monkey);
        i++;
    }
    return monkeys;
}

int print_lmb(const std::vector<Monkey> &monkeys)
{
    std::vector<uint64_t> inspected;

    for (const auto &monkey: monkeys) {
        inspected.push_back(monkey._inspected);
    }
    std::sort(inspected.begin(), inspected.end(), [](uint64_t a, uint64_t b) {return a > b;});
    std::cout << inspected[0] * inspected[1] << std::endl;
    return (0);
}

int first(std::vector<Monkey> monkeys)
{
    for (size_t i = 0; i < 20; i++) {
        for (auto &monkey: monkeys) {
            while (!monkey._starting_items_worry_level.empty()) {
                monkey.apply_operation_on_first_item();
                monkey.apply_relief_on_first_item();
                monkeys[monkey.get_next_monkey_index()]._starting_items_worry_level.push_back(monkey._starting_items_worry_level[0]);
                monkey._starting_items_worry_level.pop_front();
                monkey._inspected = monkey._inspected + 1;
            }
        }
    }
    print_lmb(monkeys);
    return (0);
}

int second(std::vector<Monkey> monkeys)
{
    uint64_t cd = 1;
    for (const auto &monkey: monkeys)
        cd *= monkey._test_condition;
    for (size_t i = 0; i < 10000; i++) {
        for (auto &monkey: monkeys) {
            while (!monkey._starting_items_worry_level.empty()) {
                monkey.apply_operation_on_first_item();
                monkey._starting_items_worry_level[0] %= cd;
                monkeys[monkey.get_next_monkey_index()]._starting_items_worry_level.push_back(monkey._starting_items_worry_level[0]);
                monkey._starting_items_worry_level.pop_front();
                monkey._inspected = monkey._inspected + 1;
            }
        }
    }
    print_lmb(monkeys);
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_11/input");
    std::vector<Monkey> monkeys = parse_file(file_content);
    first(monkeys);
    second(monkeys);
    return (0);
}
