#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

std::vector<std::string> get_file_content(const std::string &filepath);

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3
#define WALL 3

std::vector<uint8_t> cell_to_corresponding_value(const char &cell)
{
    //#.><v^
    if (cell == '>') return {RIGHT};
    if (cell == '<') return {LEFT};
    if (cell == '^') return {UP};
    if (cell == 'v') return {DOWN};
    if (cell == '#') return {WALL};
    return {};
}

std::vector<std::vector<std::vector<uint8_t>>> transform_map(const std::vector<std::string> &file_content)
{
    std::vector<std::vector<std::vector<uint8_t>>> map;

    for (const auto &line: file_content) {
        std::vector<std::vector<uint8_t>> line_map;
        for (const auto &cell: line)
            line_map.push_back(cell_to_corresponding_value(cell));
        map.push_back(line_map);
    }
    return map;
}

std::vector<std::vector<std::vector<uint8_t>>> simulate_blizzards(const std::vector<std::vector<std::vector<uint8_t>>> &map)
{
    std::vector<std::vector<std::vector<uint8_t>>> new_map(map.size(), std::vector<std::vector<uint8_t>>(map[0].size(), std::vector<uint8_t>()));

    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[y].size(); x++) {
            if (x == 0 || y == 0 || x == map[y].size() - 1 || y == map.size() - 1) {
                new_map[y][x] = map[y][x];
                continue;
            }
            for (const auto &blizzard: map[y][x]) {
                if (blizzard == RIGHT) {
                    if (x + 1 == map[y].size() - 1)
                        new_map[y][1].push_back(RIGHT);
                    else
                        new_map[y][x + 1].push_back(RIGHT);
                }
                if (blizzard == LEFT) {
                    if (x - 1 == 0)
                        new_map[y][map[y].size() - 2].push_back(LEFT);
                    else
                        new_map[y][x - 1].push_back(LEFT);
                }
                if (blizzard == DOWN) {
                    if (y + 1 == map.size() - 1)
                        new_map[1][x].push_back(DOWN);
                    else
                        new_map[y + 1][x].push_back(DOWN);
                }
                if (blizzard == UP) {
                    if (y - 1 == 0)
                        new_map[map.size() - 2][x].push_back(UP);
                    else
                        new_map[y - 1][x].push_back(UP);
                }
            }
        }
    }
    return (new_map);
}

std::set<std::pair<uint64_t,uint64_t>> update_positions(const std::vector<std::vector<std::vector<uint8_t>>> &map, const std::set<std::pair<uint64_t,uint64_t>> &actual_positions)
{
    std::set<std::pair<uint64_t,uint64_t>> future_positions;

    for (const auto &position: actual_positions) {
        if (map[position.second][position.first].empty())
            future_positions.emplace(position);
        if (position.second != 0 && map[position.second - 1][position.first].empty())
            future_positions.emplace(position.first, position.second - 1);
        if (position.second != map.size() - 1 && map[position.second + 1][position.first].empty())
            future_positions.emplace(position.first, position.second + 1);
        if (map[position.second][position.first - 1].empty())
            future_positions.emplace(position.first - 1, position.second);
        if (map[position.second][position.first + 1].empty())
            future_positions.emplace(position.first + 1, position.second);
    }
    return future_positions;
}

bool are_we_arrived(const std::set<std::pair<uint64_t,uint64_t>> &actual_positions, const std::pair<uint64_t,uint64_t> &destination)
{
    for (const auto &position: actual_positions) {
        if (position == destination)
            return true;
    }
    return false;
}

int second(std::vector<std::vector<std::vector<uint8_t>>> map)
{
    std::pair<uint64_t,uint64_t> starting_pos = std::make_pair(1,0);
    std::pair<uint64_t,uint64_t> destination = std::make_pair(map[0].size() - 2, map.size() - 1);
    std::set<std::pair<uint64_t,uint64_t>> actual_positions = {starting_pos};
    uint64_t answer = 0;
    while (!are_we_arrived(actual_positions, destination)) {
        map = simulate_blizzards(map);
        actual_positions = update_positions(map, actual_positions);
        answer++;
    }
    actual_positions = {destination};
    while (!are_we_arrived(actual_positions, starting_pos)) {
        map = simulate_blizzards(map);
        actual_positions = update_positions(map, actual_positions);
        answer++;
    }
    actual_positions = {starting_pos};
    while (!are_we_arrived(actual_positions, destination)) {
        map = simulate_blizzards(map);
        actual_positions = update_positions(map, actual_positions);
        answer++;
    }
    std::cout << answer << std::endl;
    return (0);
}

int first(std::vector<std::vector<std::vector<uint8_t>>> map)
{
    std::pair<uint64_t,uint64_t> starting_pos = std::make_pair(1,0);
    std::pair<uint64_t,uint64_t> destination = std::make_pair(map[0].size() - 2, map.size() - 1);
    std::set<std::pair<uint64_t,uint64_t>> actual_positions = {starting_pos};
    uint64_t answer = 0;
    while (!are_we_arrived(actual_positions, destination)) {
        map = simulate_blizzards(map);
        actual_positions = update_positions(map, actual_positions);
        answer++;
    }
    std::cout << answer << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_24/input");
    std::vector<std::vector<std::vector<uint8_t>>> map = transform_map(file_content);
    first(map);
    second(map);
    return (0);
}
