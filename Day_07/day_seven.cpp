#include <iostream>
#include <vector>
#include <map>
#include <limits>

std::vector<std::string> get_file_content(const std::string &filepath);

enum TYPE {
    file = 0,
    directory = 1
};

class File {
public:
    File(std::string name, TYPE type) : _name(name), _type(type) {};
    std::string _name;
    TYPE _type;
};

std::string add_paths_to_filename(std::string name, std::vector<std::string> paths, bool folder)
{
    std::string result;
    for (size_t i = 0; i < paths.size() - (uint32_t) folder; i++) {
        result.append(paths[i]);
        result.append("/");
    }
    return (result.append(name));
}

void fill_maps(std::vector<std::string> file_content, std::map<std::string, std::vector<File>> &directories, std::map<std::string, size_t> &files)
{
    size_t i = 0;
    std::vector<std::string> paths;
    while (i < file_content.size()) {
        if (file_content[i] == "$ cd ..") {
            paths.pop_back();
            i++;
            continue;
        }
        std::string dir_name = file_content[i].substr(file_content[i].find_last_of(' ') + 1, file_content[i].size() - (file_content[i].find_last_of(' ') + 1));
        std::vector<File> content;
        paths.push_back(dir_name);
        i+=2;
        for (; i < file_content.size() && file_content[i][0] != '$' ; i++) {
            std::string first = file_content[i].substr(0, file_content[i].find_first_of(' '));
            std::string second = file_content[i].substr(file_content[i].find_first_of(' ') + 1, file_content[i].size() - (file_content[i].find_first_of(' ') + 1));
            if (first == "dir")
                content.emplace_back(add_paths_to_filename(second, paths, false), TYPE::directory);
            else {
                content.emplace_back(add_paths_to_filename(second, paths, false), TYPE::file);
                files[add_paths_to_filename(second, paths, false)] = std::stoul(first);
            }
        }
        directories[add_paths_to_filename(dir_name, paths, true)] = content;
    }
}

uint32_t get_directory_size(std::vector<File> contents, std::map<std::string, std::vector<File>> directories, std::map<std::string, size_t> files)
{
    uint32_t total = 0;

    for (const auto &content: contents) {
        if (content._type == TYPE::file)
            total += files[content._name];
        else
            total += get_directory_size(directories[content._name], directories, files);
    }
    return (total);
}

int first(std::map<std::string, std::vector<File>> directories, std::map<std::string, size_t> files)
{
    uint32_t total = 0;

    for (const auto &directory: directories) {
        std::string name = std::get<0>(directory);
        std::vector<File> contents = std::get<1>(directory);
        uint32_t size = get_directory_size(contents, directories, files);
        if (size <= 100000) {
            total += size;
        }
    }
    std::cout << total << std::endl;
    return (0);
}

int second(std::map<std::string, std::vector<File>> directories, std::map<std::string, size_t> files)
{
    uint32_t size_min = 30000000 - (70000000 - get_directory_size(directories["/"], directories, files));
    uint32_t min = std::numeric_limits<uint32_t>::max();

    for (const auto &directory: directories) {
        std::string name = std::get<0>(directory);
        std::vector<File> contents = std::get<1>(directory);
        uint32_t size = get_directory_size(contents, directories, files);
        if (size > size_min && size < min) {
            min = size;
        }
    }
    std::cout << min << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_07/input");
    std::map<std::string, std::vector<File>> directories;
    std::map<std::string, size_t> files;

    fill_maps(file_content, directories, files);
    first(directories, files);
    second(directories, files);
    return (0);
}
