#include <iostream>
#include <vector>

std::vector<std::string> get_file_content(const std::string &filepath);

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

int second(const std::vector<std::vector<std::string>> &commands)
{
    std::vector<std::string> screen;//(6, std::string(40, '.'));
    std::string line;
    int cycle = 1;
    int X = 1;
    int cooldown = 0;
    int value_to_add = 0;

    for (size_t index = 0; index < commands.size(); cycle++) {
        std::cout << cycle << " " << X << std::endl;
        line.push_back((cycle % 40 >= X && cycle % 40 <= X + 2 ) ? 'X' : '.');
        if (cycle % 40 == 0) {
            screen.push_back(line);
            line = "";
        }
        if (cooldown == 1) {
            cooldown = 0;
            X += value_to_add;
            value_to_add = 0;
            continue;
        }
        if (commands[index][0] == "addx") {
            value_to_add = std::stoi(commands[index][1]);
            cooldown = 1;
        }
        index++;
    }
    for (const auto &line: screen) {
        std::cout << line << std::endl;
    }
}

int first(const std::vector<std::vector<std::string>> &commands)
{
    int cycle = 1;
    int X = 1;
    int cooldown = 0;
    int value_to_add = 0;
    int total = 0;

    for (size_t index = 0; index < commands.size(); cycle++) {
        if ((cycle - 20) % 40 == 0)
            total += X * cycle;
        if (cooldown == 1) {
            cooldown = 0;
            X += value_to_add;
            value_to_add = 0;
            continue;
        }
        if (commands[index][0] == "addx") {
            value_to_add = std::stoi(commands[index][1]);
            cooldown = 1;
        }
        index++;
    }
    std::cout << total << std::endl;
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_10/input");
    std::vector<std::vector<std::string>> commands;

    for (const auto &line: file_content) {
        commands.push_back(split(line, " "));
    }
    first(commands);
    second(commands);
    return (0);
}
