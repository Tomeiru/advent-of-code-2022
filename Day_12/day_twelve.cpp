#include <iostream>
#include <vector>
#include <limits>

std::vector<std::string> get_file_content(const std::string &filepath);

std::pair<uint64_t, uint64_t> find_character_position(std::vector<std::string> file_content, char character)
{
    for (uint32_t y = 0; y < file_content.size(); y++) {
        size_t find_value = file_content[y].find(character);
        if (find_value != std::string::npos) {
            return std::make_pair(find_value,y);
        }
    }
    return (std::make_pair(0,0));
}

void check_possible_directions(const std::vector<std::string> &file_content, std::vector<std::vector<uint64_t>> &idk, std::vector<std::pair<uint64_t, uint64_t>> &new_positions, std::pair<uint64_t, uint64_t> pos, uint64_t value)
{
    //TOP
    if (pos.second != 0 && file_content[pos.second][pos.first] + 1 >= file_content[pos.second - 1][pos.first] && idk[pos.second - 1][pos.first] == 0) {
        idk[pos.second - 1][pos.first] = value;
        new_positions.emplace_back(pos.first, pos.second - 1);
    }
    //BOTTOM
    if (pos.second != file_content.size() - 1 && file_content[pos.second][pos.first] + 1 >= file_content[pos.second + 1][pos.first] && idk[pos.second + 1][pos.first] == 0) {
        idk[pos.second + 1][pos.first] = value;
        new_positions.emplace_back(pos.first, pos.second + 1);
    }
    //LEFT
    if (pos.first != 0 && file_content[pos.second][pos.first] + 1 >= file_content[pos.second][pos.first - 1] && idk[pos.second][pos.first - 1] == 0) {
        idk[pos.second][pos.first - 1] = value;
        new_positions.emplace_back(pos.first - 1, pos.second);
    }
    //RIGHT
    if (pos.first != file_content[0].size() - 1 && file_content[pos.second][pos.first] + 1 >= file_content[pos.second][pos.first + 1] && idk[pos.second][pos.first + 1] == 0) {
        idk[pos.second][pos.first + 1] = value;
        new_positions.emplace_back(pos.first + 1, pos.second);
    }
}

uint64_t algo(const std::vector<std::string> &file_content, std::vector<std::vector<uint64_t>> idk, std::pair<uint64_t, uint64_t> end, const std::vector<std::pair<uint64_t, uint64_t>> &positions, uint64_t value)
{
    std::vector<std::pair<uint64_t, uint64_t>> new_positions;

    for (const auto &position: positions) {
        if (position == end)
            return idk[end.second][end.first];
        check_possible_directions(file_content, idk, new_positions, position, value);
    }
    if (new_positions.empty())
        return (-1);
    return (algo(file_content, idk, end, new_positions, value + 1));
}

std::vector<std::pair<uint64_t, uint64_t>> find_lowest_altitude_points(const std::vector<std::string> &file_content)
{
    std::vector<std::pair<uint64_t, uint64_t>> lowest_altitude_points;

    for (uint32_t y = 0; y < file_content.size(); y++) {
        for (uint32_t x = 0; x < file_content[y].size(); x++) {
            if (file_content[y][x] == 'a')
                lowest_altitude_points.emplace_back(x, y);
        }
    }
    return (lowest_altitude_points);
}

int first(const std::vector<std::string> &file_content, std::vector<std::vector<uint64_t>> idk, std::pair<uint64_t, uint64_t> start, std::pair<uint64_t, uint64_t> end)
{
    std::vector<std::pair<uint64_t, uint64_t>> actual(1, start);
    idk[start.second][start.first] = 1;
    uint64_t answer = algo(file_content, idk, end, actual, 2);
    std::cout << answer - 1 << std::endl;
    return (0);
}

int second(const std::vector<std::string> &file_content, std::vector<std::vector<uint64_t>> idk, std::pair<uint64_t, uint64_t> end)
{
    std::vector<std::pair<uint64_t, uint64_t>> lowest_altitude_points = find_lowest_altitude_points(file_content);
    std::uint64_t min = std::numeric_limits<uint64_t>::max();

    for (const auto &point: lowest_altitude_points) {
        std::vector<std::pair<uint64_t, uint64_t>> actual(1, point);
        idk[point.second][point.first] = 1;
        uint64_t answer = algo(file_content, idk, end, actual, 2);
        if (min > answer)
            min = answer;
        idk[point.second][point.first] = 0;
    }
    std::cout << min - 1 << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_12/input");
    std::pair<uint64_t, uint64_t> start = find_character_position(file_content, 'S');
    std::pair<uint64_t, uint64_t> end = find_character_position(file_content, 'E');
    std::vector<std::vector<uint64_t>> idk(file_content.size(), std::vector<uint64_t>(file_content[0].size(), 0));
    file_content[start.second][start.first] = 'a';
    file_content[end.second][end.first] = 'z';
    first(file_content, idk, start, end);
    second(file_content, idk, end);
    return (0);
}
