#include <iostream>
#include <vector>

#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

std::vector<std::string> get_file_content(const std::string &filepath);

std::pair<uint64_t,uint64_t> get_map_dimensions(const std::vector<std::string> &file_content)
{
    uint64_t max = 0;
    for (const auto &line: file_content) {
        if (line.empty())
            break;
        max = std::max(max, line.size() + 2);
    }
    return (std::make_pair(max, file_content.size()));
}

std::pair<std::pair<uint64_t,uint64_t>, std::vector<std::vector<uint64_t>>> get_map_and_position_from_file_content(const std::vector<std::string> &file_content)
{
    const auto &[length, height] = get_map_dimensions(file_content);
    std::vector<std::vector<uint64_t>> map(height, std::vector<uint64_t>(length, 2));
    std::pair<uint64_t,uint64_t> start = std::make_pair(1,1);
    std::pair<uint64_t,uint64_t> map_explorer = std::make_pair(1,1);
    bool is_start_set = false;

    for (const auto &line: file_content) {
        if (line.empty())
            break;
        for (const auto &cell: line) {
            if (!is_start_set && cell != ' ') {
                start.first = map_explorer.first;
                start.second = map_explorer.second;
                is_start_set = true;
            }
            if (cell == '.') map[map_explorer.second][map_explorer.first] = 0;
            if (cell == '#') map[map_explorer.second][map_explorer.first] = 1;
            map_explorer.first += 1;
        }
        map_explorer.first = 1;
        map_explorer.second += 1;
    }
    return (std::make_pair(start, map));
}

bool check_for_rotation(uint64_t &facing, const std::string &direction)
{
    if (direction == "L") {
        facing = facing == 0 ? 3 : facing - 1;
        return (true);
    }
    if (direction == "R") {
        facing = (facing + 1) % 4;
        return (true);
    }
    return (false);
}

std::pair<uint64_t,uint64_t> apply_movement(const std::vector<std::vector<uint64_t>>& map, std::pair<uint64_t,uint64_t> position, const std::pair<int64_t,int64_t> &movement)
{
    std::pair<uint64_t,uint64_t> next_pos = std::make_pair(position.first + movement.first, position.second + movement.second);

    while (map[next_pos.second][next_pos.first] == 2) {
        if (movement.first == -1 && next_pos.first == 0)
            next_pos.first = map[0].size() - 1;
        if (movement.first == 1 && next_pos.first == map[0].size() - 1)
            next_pos.first = 0;
        if (movement.second == -1 && next_pos.second == 0)
            next_pos.second = map.size() - 1;
        if (movement.second == 1 && next_pos.second == map.size() - 1)
            next_pos.second = 0;
        next_pos = std::make_pair(next_pos.first + movement.first, next_pos.second + movement.second);
    }
    if (map[next_pos.second][next_pos.first] == 2)
        std::cout << "ERROR HERE CHECK THIS OUT" << std::endl;
    if (map[next_pos.second][next_pos.first] == 1)
        return position;
    return next_pos;
}

void oob_right(std::pair<uint64_t,uint64_t> &next_pos, uint64_t &facing)
{
    std::cout << "Leaving by right side face ";
    if (next_pos.second >= 1 && next_pos.second <= 50) {
        std::cout << 2 << " to join face 4 by right side with left direction!" << std::endl;
        facing = LEFT;
        next_pos.second = (51 - next_pos.second) + 100;
        next_pos.first = 100;
        return;
    }//CORRECT
    if (next_pos.second >= 51 && next_pos.second <= 100) {
        std::cout << 3 << " to join face 2 by down side with ascending direction!" << std::endl;
        facing = UP;
        next_pos.first = 50 + next_pos.second;
        next_pos.second = 50;
        return;
    }
    if (next_pos.second >= 101 && next_pos.second <= 150) {
        std::cout << 4 << " to join face 2 by left side with right direction!" << std::endl;
        facing = LEFT;
        next_pos.second = 51 - (next_pos.second - 100);
        next_pos.first = 150;
        return;
    }
    if (next_pos.second >= 151 && next_pos.second <= 200) {
        std::cout << 6 << " to join face 4 by down side with ascending direction!" << std::endl;
        facing = UP;
        next_pos.first = next_pos.second - 100;
        next_pos.second = 150;
    }
}

void oob_up(std::pair<uint64_t,uint64_t> &next_pos, uint64_t &facing)
{
    std::cout << "Leaving by up side face ";
    if (next_pos.second == 0) {
        if (next_pos.first >= 51 && next_pos.first <= 100) {
            std::cout << 1 << " to join face 6 by left side with right direction!" << std::endl;
            facing = RIGHT;
            next_pos.second = 100 + next_pos.first;
            next_pos.first = 1;
            return;
        }
        if (next_pos.first >= 101 && next_pos.first <= 150) {
            std::cout << 2 << " to join face 6 by down side with ascending direction!" << std::endl;
            next_pos.first -= 100;
            next_pos.second = 200;
            return;
        }
    }
    if (next_pos.second == 100 && next_pos.first >= 1 && next_pos.first <= 50) {
        std::cout << "Leaving by up side face ";
        std::cout << 5 << " to join face 3 by left side with right direction!" << std::endl;
        facing = RIGHT;
        next_pos.second = 50 + next_pos.first;
        next_pos.first = 51;
    }
}

void oob_down(std::pair<uint64_t,uint64_t> &next_pos, uint64_t &facing)
{
    std::cout << "Leaving by down side face ";
    if (next_pos.second == 201) {
        std::cout << 6 << " to join face 2 by up side with descending direction!" << std::endl;
        next_pos.second = 1;
        next_pos.first += 100;
        return;
    }
    if (next_pos.second == 151) {
        std::cout << 4 << " to join face 6 by right side with left direction!" << std::endl;
        facing = LEFT;
        next_pos.second = next_pos.first + 100;
        next_pos.first = 50;
        return;
    }
    if (next_pos.second == 51) {
        std::cout << 2 << " to join face 3 by right side with left direction!" << std::endl;
        facing = LEFT;
        next_pos.second = next_pos.first - 50;
        next_pos.first = 100;
    }
}

void oob_left(std::pair<uint64_t,uint64_t> &next_pos, uint64_t &facing)
{
    std::cout << "Leaving by left side face ";
    if (next_pos.second >= 1 && next_pos.second <= 50) {
        std::cout << 1 << " to join face 5 by left side with right direction!" << std::endl;
        facing = RIGHT;
        next_pos.second = (51 - next_pos.second) + 100;
        next_pos.first = 1;
        return;
    }
    if (next_pos.second >= 51 && next_pos.second <= 100) {
        std::cout << 3 << " to join face 5 by up side with descending direction!" << std::endl;
        facing = DOWN;
        next_pos.first = next_pos.second - 50;
        next_pos.second = 101;
        std::cout << "End of this one is " << next_pos.first << " " << next_pos.second << std::endl;
        return;
    }
    if (next_pos.second >= 101 && next_pos.second <= 150) {
        std::cout << 5 << " to join face 1 by left side with right direction!" << std::endl;
        facing = RIGHT;
        next_pos.second = 51 - (next_pos.second - 100);
        next_pos.first = 51;
        return;
    }
    if (next_pos.second >= 151 && next_pos.second <= 200) {
        std::cout << 6 << " to join face 1 up side with descending direction!" << std::endl;
        facing = DOWN;
        next_pos.first = next_pos.second - 100;
        next_pos.second = 1;
    }
}


std::pair<uint64_t,uint64_t> apply_movement_second(const std::vector<std::vector<uint64_t>>& map, std::pair<uint64_t,uint64_t> position, uint64_t &facing)
{
    const std::vector<std::pair<int64_t,int64_t>> movement = {
            std::make_pair(1, 0), // RIGHT
            std::make_pair(0, 1), // DOWN
            std::make_pair(-1, 0), // LEFT
            std::make_pair(0, -1), // UP
    };
    std::pair<uint64_t,uint64_t> next_pos = std::make_pair(position.first + movement[facing].first, position.second + movement[facing].second);
    uint64_t old_facing = facing;

    std::cout << next_pos.first << " " << next_pos.second << std::endl;
    if (map[next_pos.second][next_pos.first] == 2) {
        std::cout << "Out of bounds!" << std::endl;
        switch (facing) {
            case (RIGHT):// RIGHT
                oob_right(next_pos, facing);
                break;
            case (DOWN):// DOWN
                oob_down(next_pos, facing);
                break;
            case (LEFT): // LEFT
                oob_left(next_pos, facing);
                break;
            case (UP): // UP
                oob_up(next_pos, facing);
                break;
            default:
                break;
        }
    }
    if (map[next_pos.second][next_pos.first] == 1) {
        facing = old_facing;
        return position;
    }
    return next_pos;
}

int second(const std::vector<std::vector<uint64_t>>& map, std::pair<uint64_t,uint64_t> position, const std::vector<std::string> &directions)
{
    uint64_t facing = 0;
    for (const auto &direction: directions) {
        std::cout << "Instruction is " << direction << ", I'm currently facing " << facing << " and at position " << position.first << " " << position.second << std::endl;
        /*for (uint64_t ii = 0; ii < map.size(); ii++) {
            for (uint64_t i = 0; i < map[i].size(); i++) {
                if (ii == position.second && i == position.first) {
                    std::cout << "X";
                    continue;
                }
                if (map[ii][i] == 0) std::cout << '.';
                if (map[ii][i] == 1) std::cout << '#';
                if (map[ii][i] == 2) std::cout << 'O';

            }
            std::cout << std::endl;
        }
        std::cout << std::endl;*/
        if (check_for_rotation(facing, direction))
            continue;
        uint64_t value = std::stol(direction);
        for (uint64_t i = 0; i < value; i++) {
            position = apply_movement_second(map, position, facing);
        }
    }
    std::cout << (1000 * position.second + 4 * position.first + facing) << std::endl;
}

int first(const std::vector<std::vector<uint64_t>>& map, std::pair<uint64_t,uint64_t> position, const std::vector<std::string> &directions)
{
    const std::vector<std::pair<int64_t,int64_t>> movement= {
            std::make_pair(1, 0),
            std::make_pair(0, 1),
            std::make_pair(-1, 0),
            std::make_pair(0, -1),
    };
    uint64_t facing = 0;
    for (const auto &direction: directions) {
        std::cout << direction << " " << facing << std::endl;
        /*for (uint64_t ii = 0; ii < map.size(); ii++) {
            for (uint64_t i = 0; i < map[i].size(); i++) {
                if (ii == position.second && i == position.first) {
                    std::cout << "X";
                    continue;
                }
                if (map[ii][i] == 0) std::cout << '.';
                if (map[ii][i] == 1) std::cout << '#';
                if (map[ii][i] == 2) std::cout << 'O';

            }
            std::cout << std::endl;
        }
        std::cout << std::endl;*/
        if (check_for_rotation(facing, direction))
            continue;
        uint64_t value = std::stol(direction);
        for (uint64_t i = 0; i < value; i++) {
            position = apply_movement(map, position, movement[facing]);
        }
    }
    std::cout << (1000 * position.second + 4 * position.first + facing) << std::endl;
}

std::vector<std::string> get_instructions_vector(const std::string &directions)
{
    std::vector<std::string> instructions;
    std::string value;
    for (const auto &direction: directions) {
        if (direction == 'R' || direction == 'L') {
            if (!value.empty()) instructions.push_back(value);
            instructions.emplace_back(direction == 'R' ? "R" : "L");
            value.clear();
            continue;
        }
        value.push_back(direction);
    }
    if (!value.empty()) instructions.push_back(value);
    return (instructions);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_22/input");
    std::string directions = file_content[file_content.size() - 1];
    const auto &[position, map] = get_map_and_position_from_file_content(file_content);
    std::vector<std::string> instructions = get_instructions_vector(directions);

    second(map, position, instructions);
    return (0);
}
