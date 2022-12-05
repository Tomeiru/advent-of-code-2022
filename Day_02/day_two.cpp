#include <iostream>
#include <vector>

std::vector<std::string> get_file_content(const std::string &filepath);

int first_op(std::vector<char> first, std::vector<char> second)
{
    int score = 0;

    for (int i = 0; i < first.size(); i++) {
        if (second[i] == 'X')
            score += 1;
        if (second[i] == 'Y')
            score += 2;
        if (second[i] == 'Z')
            score += 3;
        if ((first[i] == 'C' && second[i] == 'Z') || (first[i] == 'B' && second[i] == 'Y') || (first[i] == 'A' && second[i] == 'X'))
            score += 3;
        if ((first[i] == 'C' && second[i] == 'X') || (first[i] == 'A' && second[i] == 'Y') || (first[i] == 'B' && second[i] == 'Z'))
            score += 6;
    }
    std::cout << score << std::endl;
    return (0);
}

int second_op(std::vector<char> first, std::vector<char> second)
{
    int score = 0;

    for (int i = 0; i < first.size(); i++) {
        if (second[i] == 'X')
            score += 0;
        if (second[i] == 'Y')
            score += 3;
        if (second[i] == 'Z')
            score += 6;
        if ((first[i] == 'A' && second[i] == 'Y') || (first[i] == 'C' && second[i] == 'Z') || (first[i] == 'B' && second[i] == 'X'))
            score += 1;
        if ((first[i] == 'B' && second[i] == 'Y') || (first[i] == 'A' && second[i] == 'Z') || (first[i] == 'C' && second[i] == 'X'))
            score += 2;
        if ((first[i] == 'C' && second[i] == 'Y') || (first[i] == 'B' && second[i] == 'Z') || (first[i] == 'A' && second[i] == 'X'))
            score += 3;
    }
    std::cout << score << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_02/input");
    std::vector<char> first;
    std::vector<char> second;

    for (const auto &line: file_content) {
        first.push_back(line[0]);
        second.push_back(line[2]);
    }
    first_op(first, second);
    second_op(first, second);
    return (0);
}
