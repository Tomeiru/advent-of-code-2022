#include <iostream>
#include <vector>

std::vector<std::string> get_file_content(const std::string &filepath);


std::vector<std::vector<int>> file_content_to_map(std::vector<std::string> file_content)
{
    std::vector<std::vector<int>> map;
    for (const auto &line: file_content) {
        std::vector<int> map_line;
        for (size_t i = 0; i < line.length(); i++) {
            map_line.push_back(std::stoi(line.substr(i, 1)));
        }
        map.push_back(map_line);
    }
    return (map);
}

bool check_top(std::vector<std::vector<int>> map, size_t x, size_t y)
{
    int value = map[y][x];

    for (int i = y - 1; i >= 0; i--) {
        if (map[i][x] >= value)
            return (false);
    }
    return (true);
}

bool check_left(std::vector<std::vector<int>> map, size_t x, size_t y)
{
    int value = map[y][x];

    for (int i = x - 1; i >= 0; i--) {
        if (map[y][i] >= value)
            return (false);
    }
    return (true);
}

bool check_bottom(std::vector<std::vector<int>> map, size_t x, size_t y)
{
    int value = map[y][x];

    for (int i = y + 1; i < map[y].size(); i++) {
        if (map[i][x] >= value)
            return (false);
    }
    return (true);
}

bool check_right(std::vector<std::vector<int>> map, size_t x, size_t y)
{
    int value = map[y][x];

    for (int i = x + 1; i < map[y].size(); i++) {
        if (map[y][i] >= value)
            return (false);
    }
    return (true);
}

int first(std::vector<std::vector<int>> map)
{
    uint32_t total = 0;

    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[y].size(); x++) {
            if (y == 0 || y == map.size() - 1 || x == 0 || x == map[y].size() - 1) {
                total++;
                continue;
            }
            if (check_top(map, x, y) || check_bottom(map, x, y) || check_right(map, x, y) || check_left(map, x, y)) {
                total++;
            }
        }
    }
    std::cout << total << std::endl;
    return (0);
}

uint32_t view_distance_top(std::vector<std::vector<int>> map, size_t x, size_t y)
{
    int value = map[y][x];
    uint32_t ret = 0;

    for (int i = y - 1; i >= 0; i--) {
        ret++;
        if (map[i][x] >= value)
            return (ret);
    }
    return (ret);
}

uint32_t view_distance_left(std::vector<std::vector<int>> map, size_t x, size_t y)
{
    int value = map[y][x];
    uint32_t ret = 0;

    for (int i = x - 1; i >= 0; i--) {
        ret++;
        if (map[y][i] >= value)
            return (ret);
    }
    return (ret);
}

uint32_t view_distance_bottom(std::vector<std::vector<int>> map, size_t x, size_t y)
{
    int value = map[y][x];
    uint32_t ret = 0;

    for (int i = y + 1; i < map[y].size(); i++) {
        ret++;
        if (map[i][x] >= value)
            return (ret);
    }
    return (ret);
}

uint32_t view_distance_right(std::vector<std::vector<int>> map, size_t x, size_t y)
{
    int value = map[y][x];
    uint32_t ret = 0;

    for (int i = x + 1; i < map[y].size(); i++) {
        ret++;
        if (map[y][i] >= value)
            return (ret);
    }
    return (ret);
}

int second(std::vector<std::vector<int>> map)
{
    uint32_t max = 0;

    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[y].size(); x++) {
            uint32_t value = view_distance_top(map, x, y) * view_distance_bottom(map, x, y) * view_distance_left(map, x, y) * view_distance_right(map, x, y);
            if (max < value)
                max = value;
        }
    }
    std::cout << max << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_08/input");
    std::vector<std::vector<int>> map = file_content_to_map(file_content);

    second(map);
    return (0);
}
