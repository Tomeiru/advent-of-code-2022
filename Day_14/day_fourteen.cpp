#include <iostream>
#include <vector>
#include <algorithm>

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

int first(std::vector<std::vector<uint64_t>> map)
{
    bool finished = false;
    uint64_t sand_units = 0;
    while (!finished) {
        size_t x = 500;
        for (size_t y = 0; y < map.size(); y++) {
            if (y + 1 == map.size()) {
                finished = true;
                break;
            }
            if (map[y + 1][x] == 0)
                continue;
            if (map[y + 1][x - 1] == 0) {
                x--;
                continue;
            }
            if (map[y + 1][x + 1] == 0) {
                x++;
                continue;
            }
            map[y][x] = 2;
            sand_units++;
            break;
        }
    }
    for (const auto &line: map) {
        for (const auto &cell: line) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << sand_units << std::endl;
}

int second(std::vector<std::vector<uint64_t>> map, uint64_t max) {
    bool finished = false;
    uint64_t sand_units = 0;
    for (unsigned long &cell : map[max + 2]) {
        cell = 1;
    }
    while (!finished) {
        size_t x = 500;
        for (size_t y = 0; y < map.size(); y++) {
            if (map[y + 1][x] == 0)
                continue;
            if (x != 0 && map[y + 1][x - 1] == 0) {
                x--;
                continue;
            }
            if (x + 1 != map[0].size() && map[y + 1][x + 1] == 0) {
                x++;
                continue;
            }
            map[y][x] = 2;
            sand_units++;
            if (y == 0 && x == 500)
                finished = true;
            break;
        }
    }
    for (const auto &line: map) {
        for (const auto &cell: line) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << sand_units << std::endl;
}


int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_14/input");
    std::pair<uint64_t,uint64_t> value = {0,0};
    std::vector<std::vector<std::pair<uint64_t,uint64_t>>> rock_paths;
    for (const auto &line: file_content) {
        std::vector<std::pair<uint64_t,uint64_t>> rock_path;
        std::vector<std::string> array = split(line, " -> ");
        for (const auto &pair: array) {
            std::vector<std::string> values = split(pair, ",");
            std::pair rock_line = std::make_pair(std::stoul(values[0]),std::stoul(values[1]));
            rock_path.push_back(rock_line);
            if (rock_line.first > value.first)
                value.first = rock_line.first;
            if (rock_line.second > value.second)
                value.second = rock_line.second;
        }
        rock_paths.push_back(rock_path);
    }
    std::vector<std::vector<uint64_t>> map = std::vector(value.second + 5, std::vector<uint64_t>(value.first + 1000, 0));
    for (const auto &rock_path: rock_paths) {
        for (const auto &rock_line: rock_paths) {
            for (size_t i = 0; i < rock_line.size() - 1; i++) {
                bool x_different = rock_line[i].first != rock_line[i + 1].first;
                uint64_t mini = x_different ? std::min(rock_line[i].first, rock_line[i + 1].first) : std::min(rock_line[i].second, rock_line[i + 1].second);
                uint64_t maxi = x_different ? std::max(rock_line[i].first, rock_line[i + 1].first) : std::max(rock_line[i].second, rock_line[i + 1].second);
                for (size_t ii = mini; ii <= maxi; ii++) {
                    map[x_different ? rock_line[i].second : ii][x_different ? ii : rock_line[i].first] = 1;
                }
            }
        }
    }
    //first(map);
    second(map, value.second);
    return (0);
}
