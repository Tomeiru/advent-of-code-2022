#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>

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

int first(std::vector<std::vector<int>> assignment_pairs)
{
    int total = 0;

    for (const auto &assignment_pair: assignment_pairs) {
        if (assignment_pair[0] >= assignment_pair[2] && assignment_pair[1] <= assignment_pair[3])
            total++;
        else if (assignment_pair[2] >= assignment_pair[0] && assignment_pair[3] <= assignment_pair[1])
            total++;
    }
    std::cout << total << std::endl;
    return (0);
}

int second(std::vector<std::vector<int>> assignment_pairs)
{
    int total = 0;

    for (const auto &assignment_pair: assignment_pairs) {
        if (assignment_pair[1] < assignment_pair[2] && assignment_pair[0] < assignment_pair[3])
            total++;
        else if (assignment_pair[2] < assignment_pair[1] && assignment_pair[3] < assignment_pair[0])
            total++;
    }
    std::cout << 1000 - total << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_04/input");
    std::vector<std::vector<int>> assignment_pairs;

    for (const auto &line: file_content) {
        int commaIndex = line.find_first_of(',');
        int firstDashIndex = line.find_first_of('-');
        int secondDashIndex = line.find_last_of('-');
        int startA = std::stoi(line.substr(0, firstDashIndex));
        int finishA = std::stoi(line.substr(firstDashIndex + 1, commaIndex));
        int startB = std::stoi(line.substr(commaIndex + 1, secondDashIndex));
        int finishB = std::stoi(line.substr(secondDashIndex + 1, line.size()));
        std::vector<int> assignment_pair;
        assignment_pair.push_back(startA);
        assignment_pair.push_back(finishA);
        assignment_pair.push_back(startB);
        assignment_pair.push_back(finishB);
        assignment_pairs.push_back(assignment_pair);
    }
    first(assignment_pairs);
    second(assignment_pairs);
    return (0);
}
