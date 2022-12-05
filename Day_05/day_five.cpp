#include <fstream>
#include <iostream>
#include <vector>
#include <list>

std::vector<std::string> get_file_content(std::string filepath)
{
    std::ifstream file(filepath);
    std::vector<std::string> file_content;
    std::string line;
    while (std::getline(file, line)) {
        file_content.push_back(line);
    }
    file.close();
    return (file_content);
}

int print_crate_pile(const std::vector<std::list<char>> &crate_piles)
{
    for (const auto &crate_pile: crate_piles) {
        std::cout << crate_pile.back();
    }
    std::cout << std::endl;
    return (0);
}

int first(std::vector<std::vector<uint32_t>> instructions, std::vector<std::list<char>> crate_piles)
{
    for (auto & instruction : instructions) {
        for (size_t ii = 0; ii < instruction[0]; ii++) {
            crate_piles[instruction[2]].push_back(crate_piles[instruction[1]].back());
            crate_piles[instruction[1]].pop_back();
        }
    }
    print_crate_pile(crate_piles);
    return (0);
}

int second(std::vector<std::vector<uint32_t>> instructions, std::vector<std::list<char>> crate_piles)
{
    for (auto & instruction : instructions) {
        std::list<char> moving_crates;
        for (size_t ii = 0; ii < instruction[0]; ii++) {
            moving_crates.push_front(crate_piles[instruction[1]].back());
            crate_piles[instruction[1]].pop_back();
        }
        for (const auto &moving_crate: moving_crates)
            crate_piles[instruction[2]].push_back(moving_crate);
    }
    print_crate_pile(crate_piles);
    return (0);
}

int start()
{
    std::vector<std::string> instruction_file = get_file_content("../Day_05/instruction");
    std::vector<std::string> crate_file = get_file_content("../Day_05/input");
    std::vector<std::vector<uint32_t>> instructions;
    std::vector<std::list<char>> crate_piles;

    for (const auto &line: instruction_file) {
        std::vector<uint32_t> instruction;
        std::string modified_line = line;
        modified_line.erase(0,5);
        instruction.push_back(std::stoi(modified_line.substr(0, modified_line.find_first_of(' '))));
        modified_line.erase(0, modified_line.find_first_of(' ') + 1);
        modified_line.erase(0, modified_line.find_first_of(' ') + 1);
        instruction.push_back(std::stoi(modified_line.substr(0, modified_line.find_first_of(' '))) - 1);
        modified_line.erase(0, modified_line.find_first_of(' ') + 1);
        modified_line.erase(0, modified_line.find_first_of(' ') + 1);
        instruction.push_back(std::stoi(modified_line.substr(0, modified_line.find_first_of(' '))) - 1);
        instructions.push_back(instruction);
    }
    for (const auto &line: crate_file) {
        std::list<char> crate_pile;
        for (const auto &crate: line) {
            crate_pile.push_front(crate);
        }
        crate_piles.push_back(crate_pile);
    }
    first(instructions, crate_piles);
    second(instructions, crate_piles);
    return (0);
}
