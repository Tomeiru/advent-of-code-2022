#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> get_file_content(const std::string &filepath)
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