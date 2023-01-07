#include <iostream>
#include <vector>
#include <regex>

std::vector<std::string> get_file_content(const std::string &filepath);

class Blueprint {
public:
    Blueprint(uint64_t id, uint64_t ore_cost, uint64_t clay_cost, std::pair<uint64_t,uint64_t> obsidian_cost, std::pair<uint64_t,uint64_t> geode_cost) : id(id), ore_cost(ore_cost), clay_cost(clay_cost), obsidian_cost(obsidian_cost), geode_cost(geode_cost) {};
    uint64_t id;
    uint64_t ore_cost;
    uint64_t clay_cost;
    std::pair<uint64_t,uint64_t> obsidian_cost;
    std::pair<uint64_t,uint64_t> geode_cost;
};

class Statistics {
public:
    Statistics(Blueprint blueprint) : ore_robot(1), clay_robot(0), obsidian_robot(0), geode_robot(0), ore(0), clay(0), obsidian(0), geode(0), minute(1),blueprint(blueprint) {};
    uint64_t ore_robot;
    uint64_t clay_robot;
    uint64_t obsidian_robot;
    uint64_t geode_robot;
    uint64_t ore;
    uint64_t clay;
    uint64_t obsidian;
    uint64_t geode;
    uint64_t minute;
    Blueprint blueprint;
    std::bitset<4> forbidden;
    void update_resources() {
        ore += ore_robot;
        clay += clay_robot;
        obsidian += obsidian_robot;
        geode += geode_robot;
    }
    std::bitset<4> possible_creations() {
        std::bitset<4> creatable_robots("0000");

        if (ore >= blueprint.ore_cost)
            creatable_robots ^= 0b1000;
        if (ore >= blueprint.clay_cost)
            creatable_robots ^= 0b0100;
        if (ore >= blueprint.obsidian_cost.first && clay >= blueprint.obsidian_cost.second)
            creatable_robots ^= 0b0010;
        if (ore >= blueprint.geode_cost.first && obsidian >= blueprint.geode_cost.second)
            creatable_robots ^= 0b0001;
        for (size_t i = 0; i < 4; i++)
            creatable_robots[i] = creatable_robots[i] == 1 && forbidden[i] == 0;
        return (creatable_robots);
    }
};

uint64_t start_cycle(Statistics stats);

/*for (const auto &blueprint: blueprints) {
    std::cout << "Blueprint with id: " << blueprint.id << std::endl;
    std::cout << "    Needs " << blueprint.ore_cost << " ore(s) to build one ore robot" << std::endl;
    std::cout << "    Needs " << blueprint.clay_cost << " ore(s) to build one clay robot" << std::endl;
    std::cout << "    Needs " << blueprint.obsidian_cost.first << " ore(s) and " << blueprint.obsidian_cost.second << " clay(s) to build one obsidian robot" << std::endl;
    std::cout << "    Needs " << blueprint.geode_cost.first << " ore(s) and " << blueprint.geode_cost.second << " obsidian(s) to build one ore robot" << std::endl;
}*/

uint64_t craft_nothing(Statistics stats, const std::bitset<4> &creatable_robots)
{
    stats.forbidden |= creatable_robots;
    return (start_cycle(stats));
}

uint64_t craft_ore(Statistics stats)
{
    stats.forbidden = 0;
    stats.ore_robot += 1;
    stats.ore -= stats.blueprint.ore_cost;
    return (start_cycle(stats));
}

uint64_t craft_clay(Statistics stats)
{
    stats.forbidden = 0;
    stats.clay_robot += 1;
    stats.ore -= stats.blueprint.clay_cost;
    return (start_cycle(stats));
}

uint64_t craft_obsidian(Statistics stats)
{
    stats.forbidden = 0;
    stats.obsidian_robot += 1;
    stats.ore -= stats.blueprint.obsidian_cost.first;
    stats.clay -= stats.blueprint.obsidian_cost.second;
    return (start_cycle(stats));
}

uint64_t craft_geode(Statistics stats)
{
    stats.forbidden = 0;
    stats.geode_robot += 1;
    stats.ore -= stats.blueprint.geode_cost.first;
    stats.obsidian -= stats.blueprint.geode_cost.second;
    return (start_cycle(stats));
}

uint64_t craft_robots(Statistics stats, const std::bitset<4> &creatable_robots)
{
    uint64_t max = 0;

    if (creatable_robots[0] == 1)
        return (craft_geode(stats));
    if (creatable_robots[3] == 1)
        max = craft_ore(stats);
    if (creatable_robots[2] == 1)
        max = std::max(max, craft_clay(stats));
    if (creatable_robots[1] == 1)
        max = std::max(max, craft_obsidian(stats));
    return (max);
}

void debug_print(Statistics stats)
{
    std::cout << std::endl <<"Current minute: " << stats.minute << std::endl;
    std::cout << "    Current resources:" << std::endl;
    std::cout << "        Ore:" << stats.ore << std::endl;
    std::cout << "        Clay:" << stats.clay << std::endl;
    std::cout << "        Obsidian:" << stats.obsidian << std::endl;
    std::cout << "        Geode:" << stats.geode << std::endl;
    std::cout << "    Current robots:" << std::endl;
    std::cout << "        Ore:" << stats.ore_robot << std::endl;
    std::cout << "        Clay:" << stats.clay_robot << std::endl;
    std::cout << "        Obsidian:" << stats.obsidian_robot << std::endl;
    std::cout << "        Geode:" << stats.geode_robot << std::endl;
}

uint64_t start_cycle(Statistics stats)
{
    //debug_print(stats);
    if (stats.minute == 33)
        return (stats.geode);
    std::bitset<4> creatable_robots = stats.possible_creations();
    stats.minute += 1;
    stats.update_resources();
    if (creatable_robots == 0b0000)
        return (start_cycle(stats));
    return (std::max(craft_nothing(stats, creatable_robots), craft_robots(stats, creatable_robots)));
}

uint64_t determine_quality_level(Blueprint blueprint)
{
    Statistics stats(blueprint);
    return (start_cycle(stats));
}

int first(const std::vector<Blueprint> &blueprints)
{
    uint64_t total = 0;

    for (const auto &blueprint: blueprints) {
        std::cout << "Starting " << blueprint.id << std::endl;
        total += blueprint.id * determine_quality_level(blueprint);
        std::cout << "Total is now " << total << std::endl;
    }
    std::cout << total << std::endl;
    return (0);
}

int second(const std::vector<Blueprint> &blueprints)
{
    uint64_t total = 1;

    for (const auto &blueprint: blueprints) {
        std::cout << "Starting " << blueprint.id << std::endl;
        total *= determine_quality_level(blueprint);
        std::cout << "Total is now " << total << std::endl;
    }
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_19/input");
    std::vector<Blueprint> blueprints;
    std::regex regex("Blueprint (\\d+): Each ore robot costs (\\d+) ore. Each clay robot costs (\\d+) ore. Each obsidian robot costs (\\d+) ore and (\\d+) clay. Each geode robot costs (\\d+) ore and (\\d+) obsidian.");
    std::smatch matches;

    for (const auto &line: file_content) {
        if (std::regex_search(line, matches, regex)) {
            uint64_t id = std::stoul(matches[1]);
            uint64_t ore_cost = std::stoul(matches[2]);
            uint64_t clay_cost = std::stoul(matches[3]);
            std::pair<uint64_t,uint64_t> obsidian_cost = std::make_pair(std::stoul(matches[4]), std::stoul(matches[5]));
            std::pair<uint64_t,uint64_t> geode_cost = std::make_pair(std::stoul(matches[6]), std::stoul(matches[7]));
            blueprints.emplace_back(id, ore_cost, clay_cost, obsidian_cost, geode_cost);
        }
    }
    second(blueprints);
    return (0);
}
