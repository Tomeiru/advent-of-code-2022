#include <iostream>
#include <vector>
#include <set>
#include <map>

std::vector<std::string> get_file_content(const std::string &filepath);

const std::map<char, std::pair<int32_t, int32_t>> charToDirectionValues = {
        std::make_pair('U', std::make_pair(0, -1)),
        std::make_pair('D', std::make_pair(0, +1)),
        std::make_pair('L', std::make_pair(-1, 0)),
        std::make_pair('R', std::make_pair(+1, 0)),
};

void move_head(std::pair<int32_t, int32_t> &head_position, char direction)
{
    head_position.first += charToDirectionValues.find(direction)->second.first;
    head_position.second += charToDirectionValues.find(direction)->second.second;
}

bool are_head_tail_touching(const std::pair<int32_t, int32_t> &head_position, const std::pair<int32_t, int32_t> &tail_position)
{
    if (head_position.first == tail_position.first && head_position.second == tail_position.second)
        return (true);
    if (head_position.first == tail_position.first)
        return abs(head_position.second - tail_position.second) == 1;
    if (head_position.second == tail_position.second)
        return abs(head_position.first - tail_position.first) == 1;
    return abs(head_position.first - tail_position.first) == 1 && abs(head_position.second - tail_position.second) == 1;
}

void move_tail(std::pair<int32_t, int32_t> &tail_position, const std::pair<int32_t, int32_t> &head_position)
{
    std::pair<int32_t, int32_t> movement = {0, 0};

    if (head_position.first == tail_position.first)
        movement.second = head_position.second - tail_position.second < 0 ? -1 : 1;
    else if (head_position.second == tail_position.second)
        movement.first = head_position.first - tail_position.first < 0 ? -1 : 1;
    else {
        movement.first = head_position.first - tail_position.first < 0 ? -1 : 1;
        movement.second = head_position.second - tail_position.second < 0 ? -1 : 1;
    }
    tail_position.first += movement.first;
    tail_position.second += movement.second;
}

int first(const std::vector<std::pair<char, uint32_t>>& directions)
{
    std::pair<int32_t, int32_t> head_position = {0, 0};
    std::pair<int32_t, int32_t> tail_position = {0, 0};
    std::set<std::pair<int32_t, int32_t>> traveled;

    for (const auto &direction: directions) {
        for (uint32_t i = 0; i < direction.second; i++) {
            move_head(head_position, direction.first);
            if (!are_head_tail_touching(head_position, tail_position))
                move_tail(tail_position, head_position);
            traveled.insert(tail_position);
        }
    }
    std::cout << traveled.size() << std::endl;
    return (0);
}

int second(const std::vector<std::pair<char, uint32_t>>& directions)
{
    std::vector<std::pair<int32_t, int32_t>> rope(10, {0, 0});
    std::set<std::pair<int32_t, int32_t>> traveled;

    for (const auto &direction: directions) {
        for (uint32_t i = 0; i < direction.second; i++) {
            move_head(rope[0], direction.first);
            for (uint32_t rope_index = 1; rope_index < rope.size(); rope_index++) {
                if (!are_head_tail_touching(rope[rope_index - 1], rope[rope_index]))
                    move_tail(rope[rope_index], rope[rope_index - 1]);
            }
            traveled.insert(rope[9]);
        }
    }
    std::cout << traveled.size() << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_09/input");
    std::vector<std::pair<char, uint32_t>> directions;

    for (const auto &line: file_content) {
        char direction = line[0];
        uint32_t movement = std::stoul(line.substr(line.find_first_of(' ') + 1, line.length() - line.find_first_of(' ') + 1));
        directions.emplace_back(direction, movement);
    }
    first(directions);
    second(directions);
    return (0);
}
