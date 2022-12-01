#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

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

std::vector<unsigned long> get_sorted_elves(std::vector<std::string> file_content)
{
    std::vector<unsigned long> elves;
    unsigned long total = 0;

    for (size_t i = 0; i < file_content.size(); i++) {
        if (!file_content[i].empty()) {
            total += std::stoi(file_content[i]);
            continue;
        }
        elves.push_back(total);
        total = 0;
    }
    std::sort(elves.begin(), elves.end(), [](int a, int b) {return a > b;});
    return (elves);

}

void first(std::vector<unsigned long> sorted_elves)
{
    unsigned long result = sorted_elves[0];
    std::cout << result << std::endl;
}

void second(std::vector<unsigned long> sorted_elves)
{
    unsigned long result = 0;
    for (size_t i = 0; i < 3; i++) {
        result += sorted_elves[i];
    }
    std::cout << result << std::endl;
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_01/input");
    std::vector<unsigned long> sorted_elves = get_sorted_elves(file_content);
    first(sorted_elves);
    second(sorted_elves);
    return (0);
}
