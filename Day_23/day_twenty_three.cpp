#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <algorithm>

std::vector<std::string> get_file_content(const std::string &filepath);

std::pair<std::vector<std::vector<uint64_t>>,std::vector<std::pair<uint64_t,uint64_t>>> handle_input(const std::vector<std::string> &file_content)
{
    std::vector<std::vector<uint64_t>> map(file_content.size() + 1000, std::vector<uint64_t>(file_content[0].size() + 1000, 0));
    std::vector<std::pair<uint64_t,uint64_t>> elves_positions;

    for (size_t y = 0; y < file_content.size(); y++) {
        for (size_t x = 0; x < file_content[y].size(); x++) {
            if (file_content[y][x] == '#') {
                map[y + 500][x + 500] = 1;
                elves_positions.emplace_back(x + 500, y + 500);
            }
        }
    }
    return std::make_pair(map, elves_positions);
}

std::pair<std::array<std::pair<int32_t, int32_t>, 4>,std::array<std::array<std::pair<int32_t,int32_t>,3>,4>> movement_array_initializer()
{
    const std::array<std::pair<int32_t, int32_t>, 4> movement = {
            std::make_pair(0, -1), //NORTH
            std::make_pair(0, 1), //SOUTH
            std::make_pair(-1, 0), //WEST
            std::make_pair(1, 0) //EAST
    };
    const std::array<std::array<std::pair<int32_t,int32_t>,3>,4> cells_to_check = {{
        {
            std::make_pair(-1, -1), //NORTH-WEST
            std::make_pair(0, -1), //NORTH
            std::make_pair(1, -1) //NORTH-EAST
        },
        {
            std::make_pair(-1, 1), //SOUTH-WEST
            std::make_pair(0, 1), //SOUTH
            std::make_pair(1, 1), //SOUTH-EAST
        },
        {
            std::make_pair(-1, -1), //SOUTH-WEST
            std::make_pair(-1, 0), //WEST
            std::make_pair(-1, 1), //NORTH-WEST
        },
        {
            std::make_pair(1, -1), //NORTH-EAST
            std::make_pair(1, 0), //EAST
            std::make_pair(1, 1), //SOUTH-EAST
        },
    }};
    return (std::make_pair(movement, cells_to_check));
}

bool are_adjacent_positions_empty(const std::vector<std::vector<uint64_t>> &map, const std::pair<uint64_t, uint64_t> &elf_position)
{
    const std::array<std::pair<int32_t, int32_t>, 8> adjacent_positions = {
            std::make_pair(0, -1), //NORTH
            std::make_pair(0, 1), //SOUTH
            std::make_pair(-1, 0), //WEST
            std::make_pair(1, 0), //EAST
            std::make_pair(-1, -1), //NORTH-WEST
            std::make_pair(1, -1), //NORTH-EAST
            std::make_pair(-1, 1), //SOUTH-WEST
            std::make_pair(1, 1), //SOUTH-EAST
    };

    for (const auto &adjacent_position: adjacent_positions) {
        if (map[elf_position.second + adjacent_position.second][elf_position.first + adjacent_position.first] == 1)
            return (false);
    }
    return (true);
}

bool are_positions_empty(const std::vector<std::vector<uint64_t>> &map, const std::pair<uint64_t, uint64_t> &elf_position, const std::array<std::pair<int32_t,int32_t>,3> &positions)
{
    for (const auto &position: positions) {
        if (map[elf_position.second + position.second][elf_position.first + position.first] == 1)
            return (false);
    }
    return (true);
}

std::pair<uint64_t,uint64_t> apply_movement(const std::pair<uint64_t,uint64_t> &pos, const std::pair<int64_t,int64_t> &movement)
{
    return (std::make_pair(pos.first + movement.first, pos.second + movement.second));
}

void proposition_part(const std::vector<std::vector<uint64_t>> &map, std::vector<std::pair<uint64_t,uint64_t>> elves_positions, std::vector<bool> &moving, const uint8_t &movement_index, std::map<std::pair<uint64_t,uint64_t>, uint64_t> &proposed_destinations)
{
    const auto &[movement, cells_to_check] = movement_array_initializer();

    for (uint64_t elf_id = 0; elf_id < elves_positions.size(); elf_id++) {
        if (are_adjacent_positions_empty(map, elves_positions[elf_id]))
            continue;
        for (size_t i = 0; i < 4; i++) {
            if (are_positions_empty(map, elves_positions[elf_id], cells_to_check[(movement_index + i) % 4])) {
                std::pair<uint64_t,uint64_t> proposed_destination = apply_movement(elves_positions[elf_id], movement[(movement_index + i) % 4]);
                bool is_unique = proposed_destinations.insert(std::make_pair(proposed_destination, elf_id)).second;
                moving[proposed_destinations[proposed_destination]] = is_unique;
                break;
            }
        }
    }
}

void selection_part(std::vector<std::vector<uint64_t>> &map, std::vector<std::pair<uint64_t,uint64_t>> &elves_positions, const std::map<std::pair<uint64_t,uint64_t>, uint64_t> &proposed_destinations, const std::vector<bool> &moving)
{
    for (const auto &[new_destination, elf_id]: proposed_destinations) {
        if (!moving[elf_id])
            continue;
        map[elves_positions[elf_id].second][elves_positions[elf_id].first] = 0;
        map[new_destination.second][new_destination.first] = 1;
        elves_positions[elf_id] = new_destination;
    }
}

bool simulate_round(std::vector<std::vector<uint64_t>> &map, std::vector<std::pair<uint64_t,uint64_t>> &elves_positions, const uint8_t &movement_index)
{
    std::vector<bool> moving(elves_positions.size(), false);
    std::map<std::pair<uint64_t,uint64_t>, uint64_t> proposed_destinations;
    proposition_part(map, elves_positions, moving, movement_index, proposed_destinations);
    selection_part(map, elves_positions, proposed_destinations, moving);
    for (const auto &movement: moving) {
        if (movement)
            return (true);
    }
    return (false);
}

uint64_t simulate_rounds_until_success(std::vector<std::vector<uint64_t>> map, std::vector<std::pair<uint64_t,uint64_t>> elves_positions)
{
    uint8_t movement_index = 0;
    uint64_t round_number = 0;
    while (simulate_round(map, elves_positions, movement_index)) {
        movement_index = (movement_index + 1) % 4;
        round_number++;
    }
    return (round_number + 1);
}

std::vector<std::vector<uint64_t>> simulate_rounds(std::vector<std::vector<uint64_t>> map, std::vector<std::pair<uint64_t,uint64_t>> elves_positions)
{
    uint8_t movement_index = 0;
    for (size_t round = 0; round < 10; round++) {
        simulate_round(map, elves_positions, movement_index);
        movement_index = (movement_index + 1) % 4;
    }
    return (map);
}

std::vector<std::vector<uint64_t>> remove_unnecessary_line(const std::vector<std::vector<uint64_t>> &map)
{
    size_t first_line = 0;
    size_t last_line = map.size() - 1;
    std::vector<std::vector<uint64_t>> trimmed_map;

    for (; first_line < map.size(); first_line++) {
        if (std::find(map[first_line].begin(), map[first_line].end(), 1) == map[first_line].end())
            continue;
        break;
    }
    for (; last_line > first_line; last_line--) {
        if (std::find(map[last_line].begin(), map[last_line].end(), 1) == map[last_line].end())
            continue;
        break;
    }
    for (size_t i = first_line; i <= last_line; i++)
        trimmed_map.push_back(map[i]);
    return (trimmed_map);

}

std::vector<std::vector<uint64_t>> trim_map(const std::vector<std::vector<uint64_t>> &map)
{
    std::vector<std::vector<uint64_t>> trimmed_map = remove_unnecessary_line(map);
    size_t start = trimmed_map[0].size();
    size_t end = 0;
    for (size_t i = 0; i < trimmed_map.size(); i++) {
        size_t start_i = 0;
        size_t end_i = trimmed_map[0].size();
        for ( ; start_i < trimmed_map[i].size(); start_i++) {
            if (trimmed_map[i][start_i] != 1)
                continue;
            break;
        }
        for ( ; end_i > 0; end_i--) {
            if (trimmed_map[i][end_i - 1] != 1)
                continue;
            break;
        }
        start = std::min(start, start_i);
        end = std::max(end, end_i);
    }
    for (auto &line : trimmed_map) {
        line = std::vector<uint64_t>(line.begin() + start, line.begin() + end);
    }
    return (trimmed_map);
}

uint64_t count_empty_space(const std::vector<std::vector<uint64_t>> &trimmed_map)
{
    uint64_t result = 0;
    for (const auto &line: trimmed_map) {
        result += std::count(line.begin(), line.end(), 0);
    }
    return (result);
}

int first(const std::vector<std::vector<uint64_t>> &map, const std::vector<std::pair<uint64_t,uint64_t>> &elves_positions)
{
    std::vector<std::vector<uint64_t>> result_map = simulate_rounds(map, elves_positions);
    std::vector<std::vector<uint64_t>> trimmed_map = trim_map(result_map);
    uint64_t result = count_empty_space(trimmed_map);

    std::cout << result << std::endl;
    return (0);
}

int second(const std::vector<std::vector<uint64_t>> &map, const std::vector<std::pair<uint64_t,uint64_t>> &elves_positions)
{
    uint64_t result = simulate_rounds_until_success(map, elves_positions);
    std::cout << result << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_23/input");
    const auto &[map, elves_positions] = handle_input(file_content);

    first(map, elves_positions);
    second(map, elves_positions);
    return (0);
}
