#include <iostream>
#include <vector>
#include <deque>
#include <bitset>
#include <array>
#include <map>

std::vector<std::string> get_file_content(const std::string &filepath);

std::array<std::deque<std::bitset<7>>,5> get_rocks()
{
    std::array<std::deque<std::bitset<7>>,5> rocks = {{
        {
            std::bitset<7>("0011110"),
        },
        {
            std::bitset<7>("0001000"),
            std::bitset<7>("0011100"),
            std::bitset<7>("0001000"),
        },
        {
            std::bitset<7>("0000100"),
            std::bitset<7>("0000100"),
            std::bitset<7>("0011100"),
        },
        {
            std::bitset<7>("0010000"),
            std::bitset<7>("0010000"),
            std::bitset<7>("0010000"),
            std::bitset<7>("0010000"),
        },
        {
            std::bitset<7>("0011000"),
            std::bitset<7>("0011000"),
        },
    }};
    return (rocks);
}

std::deque<std::bitset<7>> move_right(const std::deque<std::bitset<7>> &rock, const std::deque<std::bitset<7>> &map, const uint64_t top)
{
    std::deque<std::bitset<7>> shifted_rock;
    for (size_t i = 0; i < rock.size(); i++) {
        if ((std::bitset<7>("0000001") & rock[i]) == std::bitset<7>("0000001"))
            return(rock);
        shifted_rock.push_back(rock[i] >> 1);
        if ((map[top + i] & shifted_rock[i]) != std::bitset<7>("0000000"))
            return (rock);
    }
    return (shifted_rock);
}

std::deque<std::bitset<7>> move_left(const std::deque<std::bitset<7>> &rock, const std::deque<std::bitset<7>> &map, const uint64_t top)
{
    std::deque<std::bitset<7>> shifted_rock;
    for (size_t i = 0; i < rock.size(); i++) {
        if ((std::bitset<7>("1000000") & rock[i]) == std::bitset<7>("1000000"))
            return(rock);
        shifted_rock.push_back(rock[i] << 1);
        if ((map[top + i] & shifted_rock[i]) != std::bitset<7>("0000000"))
            return (rock);
    }
    return (shifted_rock);
}

bool move_bottom(const std::deque<std::bitset<7>> &rock, std::deque<std::bitset<7>> &map, uint64_t &top, uint64_t &bottom)
{
    if (bottom == map.size() - 1)
        return (false);
    for (uint64_t i = 0; top + i <= bottom; i++) {
        if ((map[top + i + 1] & rock[i]) != std::bitset<7>("0000000"))
            return (false);
    }
    if (top == 0 && map[0] == std::bitset<7>("0000000"))
        map.pop_front();
    else {
        top += 1;
        bottom += 1;
    }
    return (true);
}

void add_rock_to_map(const std::deque<std::bitset<7>> &rock, std::deque<std::bitset<7>> &map, uint64_t &top, uint64_t &bottom)
{
    for (int64_t i = 0; top + i <= bottom; i++) {
        map[top + i] |= rock[i];
    }
}

int second(const std::string &jet_pattern, const std::array<std::deque<std::bitset<7>>,5> &rocks)
{
    std::deque<std::bitset<7>> map;
    uint64_t jet_tracker = 0;
    uint64_t height = 0;
    std::map<std::tuple<uint64_t, uint64_t, uint64_t>,std::pair<uint64_t, uint64_t>> cycle_analyzer;
    uint64_t it_to_match = 10;
    uint64_t cycle_to_do = 1000000;
    uint64_t bonus_height = 0;
    bool does_cycle_matter = true;

    for (size_t i = 0; i < cycle_to_do; i++) {
        std::deque<std::bitset<7>> rock = rocks[i % 5];
        for (uint64_t j = 0; j < rock.size() + 3; j++)
            map.emplace_front("0000000");
        uint64_t top = 0;
        uint64_t bottom = rock.size() - 1;
        do {
            char jet = jet_pattern[jet_tracker++ % jet_pattern.size()];
            rock = (jet == '>' ? move_right(rock, map, top) : move_left(rock, map, top));
        } while (move_bottom(rock, map, top, bottom));
        add_rock_to_map(rock, map, top, bottom);
        std::tuple<uint64_t, uint64_t, uint64_t> it_info = std::make_tuple(map.size() - height, (jet_tracker - 1) % jet_pattern.size(), i % 5);
        height = map.size();
        it_to_match = (!cycle_analyzer.insert(std::make_pair(it_info,std::make_pair(i, height))).second) ? it_to_match - 1 : 10;
        if (does_cycle_matter && it_to_match == 0) {
            does_cycle_matter = false;
            uint64_t cycle_duration = i - cycle_analyzer[it_info].first;
            uint64_t cycle_height = height - cycle_analyzer[it_info].second;
            uint64_t cycle_remaining = (1000000000000 - i) % cycle_duration;
            uint64_t cycle_applied = (1000000000000 - i) / cycle_duration;
            std::cout << "Found a cycle: " << std::endl;
            std::cout << "    first i = " << cycle_analyzer[it_info].first << std::endl;
            std::cout << "    actual i = " << i << std::endl;
            std::cout << "Cycle duration: " << cycle_duration << std::endl;
            std::cout << "Cycle height: " << cycle_height << std::endl;
            std::cout << "Cycle applied: " << cycle_applied << std::endl;
            std::cout << "Cycle remaining: " << cycle_remaining << std::endl;
            bonus_height += cycle_applied * cycle_height;
            cycle_to_do = i + cycle_remaining;
        }
    }
    std::cout << height + bonus_height << std::endl;
    return (0);
}

int first(const std::string &jet_pattern, const std::array<std::deque<std::bitset<7>>,5> &rocks)
{
    std::deque<std::bitset<7>> map;
    uint64_t jet_tracker = 0;
    uint64_t height = 0;

    for (size_t i = 0; i < 2022; i++) {
        std::deque<std::bitset<7>> rock = rocks[i % 5];
        for (uint64_t j = 0; j < rock.size() + 3; j++)
            map.emplace_front("0000000");
        uint64_t top = 0;
        uint64_t bottom = rock.size() - 1;
        do {
            char jet = jet_pattern[jet_tracker++ % jet_pattern.size()];
            rock = (jet == '>' ? move_right(rock, map, top) : move_left(rock, map, top));
        } while (move_bottom(rock, map, top, bottom));
        add_rock_to_map(rock, map, top, bottom);
        height = map.size();
    }
    std::cout << height << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_17/input");
    std::array<std::deque<std::bitset<7>>,5> rocks = get_rocks();

    first(file_content[0], rocks);
    second(file_content[0], rocks);
    return (0);
}
