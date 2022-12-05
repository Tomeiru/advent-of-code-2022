#include <iostream>
#include <vector>

std::vector<std::string> get_file_content(const std::string &filepath);

int count_common_characters(std::vector<char> common_characters)
{
    int total = 0;

    for (const auto &common_character: common_characters) {
        if (islower(common_character))
            total += common_character - 97 + 1;
        if (isupper(common_character))
            total += common_character - 65 + 27;
    }
    return (total);
}

int first(std::vector<std::string> file_content)
{
    std::vector<std::string> first_half;
    std::vector<std::string> second_half;
    std::vector<char> common_characters;
    int total = 0;

    for (const auto &line: file_content) {
        first_half.push_back(line.substr(0, line.size()/2));
        second_half.push_back(line.substr(line.size()/2, line.size()/2));
    }
    for (int i = 0; i < first_half.size(); i++) {
        for (const auto &chara: first_half[i]) {
            if ((second_half[i].find(chara) == std::string::npos))
                continue;
            common_characters.push_back(chara);
            break;
        }
    }
    total = count_common_characters(common_characters);
    std::cout << total << std::endl;
    return (0);
}


int second(std::vector<std::string> file_content)
{
    std::vector<std::string> first;
    std::vector<std::string> second;
    std::vector<std::string> third;
    std::vector<char> common_characters;
    int total = 0;

    for (int i = 0; i < file_content.size() - 2; i += 3) {
        first.push_back(file_content[i]);
        second.push_back(file_content[i + 1]);
        third.push_back(file_content[i + 2]);
    }
    for (int i = 0; i < first.size(); i++) {
        for (const auto &elem: first[i]) {
            if (second[i].find(elem) == std::string::npos || third[i].find(elem) == std::string::npos)
                continue;
            common_characters.push_back(elem);
            break;
        }
    }
    total = count_common_characters(common_characters);
    std::cout << total << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_03/input");
    first(file_content);
    second(file_content);
    return (0);
}
