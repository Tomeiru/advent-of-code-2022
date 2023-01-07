#include <iostream>
#include <vector>

std::vector<std::string> get_file_content(const std::string &filepath);

bool is_all_char_unique(std::string str)
{
    for (size_t i = 0; i < str.size(); i++) {
        for (size_t ii = 0; ii < str.size(); ii++) {
            if (ii == i)
                continue;
            if (str[i] == str[ii])
                return (false);
        }
    }
    return (true);
}

int first_and_second(std::string line, size_t size)
{
    size_t i = 0;

    for (; i + size - 1 < line.size(); i++) {
        std::string substr = line.substr(i,  size);
        if (is_all_char_unique(substr))
            break;
    }
    std::cout << i + size << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_06/input");
    first_and_second(file_content[0], 4);
    first_and_second(file_content[0], 14);
    return (0);
}