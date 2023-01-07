#include <iostream>
#include <utility>
#include <vector>
#include <regex>
#include <map>

std::vector<std::string> get_file_content(const std::string &filepath);
uint64_t human_moving(uint64_t actual_human, uint64_t actual_elephant, uint64_t cooldown_human, uint64_t cooldown_elephant, const std::vector<uint64_t> &important_valves, std::map<uint64_t,bool> targetted, std::map<uint64_t,bool> opened, const std::vector<uint64_t> &flow_rate, const std::map<uint64_t,std::map<uint64_t,uint64_t>> &all_distances, uint64_t release_rate, uint64_t released, int64_t remaining, uint64_t not_opened, std::vector<uint64_t> precedent_choice);

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

std::vector<uint64_t> get_important_valves(const std::vector<uint64_t> &flow_rate, uint64_t starting_point)
{
    std::vector<uint64_t> important_valves(1, starting_point);

    for (size_t i = 0; i < flow_rate.size(); i++) {
        if (flow_rate[i] > 0)
            important_valves.push_back(i);
    }
    return (important_valves);
}

uint64_t calculate_distance(uint64_t starting_point, uint64_t destination, const std::vector<std::vector<uint64_t>> &adjacents, const std::map<uint64_t, std::string> &id_to_str)
{
    uint64_t distance = 0;
    std::vector<uint64_t> places_to_check(1, starting_point);
    std::vector<uint64_t> next_places_to_check;

    while (1) {
        for (const auto &place: places_to_check) {
            if (place == destination)
                return distance;
            for (const auto adjacent: adjacents[place])
                next_places_to_check.push_back(adjacent);
        }
        distance += 1;
        places_to_check = next_places_to_check;
        next_places_to_check.clear();
    }
    return (0);
}

std::map<uint64_t,uint64_t> calculate_distances(uint64_t id, const std::vector<uint64_t> &important_valves, const std::vector<std::vector<uint64_t>> &adjacents, const std::map<uint64_t, std::string> &id_to_str)
{
    std::map<uint64_t,uint64_t> distances;

    for (const auto &valve: important_valves) {
        if (valve == id)
            continue;
        distances.emplace(valve, calculate_distance(id, valve, adjacents, id_to_str));
    }
    return (distances);
}

std::map<uint64_t,std::map<uint64_t,uint64_t>> calculate_all_distances(const std::vector<uint64_t> &important_valves, const std::vector<std::vector<uint64_t>> &adjacents, const std::map<uint64_t, std::string> &id_to_str)
{
    std::map<uint64_t,std::map<uint64_t,uint64_t>> all_distances;

    for (const auto &valve: important_valves) {
        all_distances.emplace(valve, calculate_distances(valve, important_valves, adjacents, id_to_str));
    }
    return (all_distances);
}

void print_precedent(const std::vector<uint64_t> &precedent_choice)
{
    std::cout << "[ ";
    for (const auto &choice: precedent_choice) {
        std::cout << choice << " ";
    }
    std::cout << "]";
}

uint64_t bruteforce_result(uint64_t actual, const std::vector<uint64_t> &important_valves, std::map<uint64_t,bool> opened, const std::vector<uint64_t> &flow_rate, const std::map<uint64_t,std::map<uint64_t,uint64_t>> &all_distances, uint64_t release_rate, uint64_t released, uint64_t cooldown, int64_t remaining, uint64_t not_opened, std::vector<uint64_t> precedent_choice)
{
    if (remaining == 0)
        return released;
    if (not_opened == 0) {
        return released + remaining * release_rate;
    }
    /*std::cout << std::endl << "Minute " << 31 - remaining << std::endl;
    print_precedent(precedent_choice);
    std::cout << " " << released << "+" << release_rate << " ";*/
    released += release_rate;
    //std::cout << released << std::endl;
    remaining -= 1;
    if (cooldown > 0) {
        //std::cout << "On cooldown" << std::endl;
        return (bruteforce_result(actual, important_valves, opened, flow_rate, all_distances, release_rate, released,
                                  cooldown - 1, remaining, not_opened, precedent_choice));
    }
    if (!opened[actual]) {
        opened[actual] = true;
        //std::cout << "Opened " << actual << std::endl;
        return (bruteforce_result(actual, important_valves, opened, flow_rate, all_distances, release_rate + flow_rate[actual], released, cooldown, remaining, not_opened - 1, precedent_choice));
    }
    uint64_t max = 0;
    for (const auto &valve: important_valves) {
        if (valve == actual || opened[valve])
            continue;
        precedent_choice.push_back(valve);
        max = std::max(max, bruteforce_result(valve, important_valves, opened, flow_rate, all_distances, release_rate, released, all_distances.at(actual).at(valve) - 1, remaining, not_opened, precedent_choice));
        precedent_choice.pop_back();
    }
    return (max);
}

std::map<uint64_t,bool> create_opened_map(const std::vector<uint64_t> &important_valves, uint64_t starting_point)
{
    std::map<uint64_t,bool> opened;

    for (const auto &valve: important_valves) {
        if (valve == starting_point)
            opened.emplace(valve, true);
        else
            opened.emplace(valve, false);
    }
    return (opened);
}

uint64_t elephant_moving(uint64_t actual_human, uint64_t actual_elephant, uint64_t cooldown_human, uint64_t cooldown_elephant, const std::vector<uint64_t> &important_valves, std::map<uint64_t,bool> targetted, std::map<uint64_t,bool> opened, const std::vector<uint64_t> &flow_rate, const std::map<uint64_t,std::map<uint64_t,uint64_t>> &all_distances, uint64_t release_rate, uint64_t released, int64_t remaining, uint64_t not_opened, std::vector<uint64_t> precedent_choice)
{
    /*std::cout << std::endl << "Elephant moving at minute " << 27 - remaining << std::endl;
    print_precedent(precedent_choice);
    std::cout << " " << released << "+" << release_rate << " ";*/
    remaining -= 1;
    if (not_opened == 0)
        return human_moving(actual_human, actual_elephant, cooldown_human, cooldown_elephant, important_valves, targetted, opened, flow_rate, all_distances, release_rate, released, remaining, not_opened, precedent_choice);
    if (cooldown_elephant > 0) {
        //std::cout << "On cooldown" << std::endl;
        return human_moving(actual_human, actual_elephant, cooldown_human, cooldown_elephant - 1, important_valves,
                            targetted, opened, flow_rate, all_distances, release_rate, released, remaining, not_opened, precedent_choice);
    }
    if (!opened[actual_elephant]) {
        //std::cout << "Opened " << actual_elephant << std::endl;
        opened[actual_elephant] = true;
        return human_moving(actual_human, actual_elephant, cooldown_human, cooldown_elephant, important_valves, targetted, opened, flow_rate, all_distances, release_rate + flow_rate[actual_elephant], released, remaining, not_opened - 1, precedent_choice);
    }
    uint64_t max = 0;
    for (const auto &valve: important_valves) {
        if (valve == actual_elephant || targetted[valve]  || all_distances.at(actual_elephant).at(valve) - 1 >= remaining)
            continue;
        targetted[valve] = true;
        precedent_choice.push_back(valve);
        max = std::max(max, human_moving(actual_human, valve, cooldown_human, all_distances.at(actual_elephant).at(valve) - 1, important_valves, targetted, opened, flow_rate, all_distances, release_rate, released, remaining, not_opened, precedent_choice));
        targetted[valve] = false;
        precedent_choice.pop_back();
    }
    if (max == 0)
        max = human_moving(actual_human, actual_elephant, cooldown_human, cooldown_elephant, important_valves, targetted, opened, flow_rate, all_distances, release_rate, released, remaining, not_opened, precedent_choice);
    return (max);
}

uint64_t human_moving(uint64_t actual_human, uint64_t actual_elephant, uint64_t cooldown_human, uint64_t cooldown_elephant, const std::vector<uint64_t> &important_valves, std::map<uint64_t,bool> targetted, std::map<uint64_t,bool> opened, const std::vector<uint64_t> &flow_rate, const std::map<uint64_t,std::map<uint64_t,uint64_t>> &all_distances, uint64_t release_rate, uint64_t released, int64_t remaining, uint64_t not_opened, std::vector<uint64_t> precedent_choice)
{
    /*std::cout << std::endl << "Human moving at minute " << 27 - remaining << std::endl;
    print_precedent(precedent_choice);
    std::cout << " " << released << "+" << release_rate << " ";*/
    if (remaining == 0)
        return released;
    if (not_opened == 0)
        return released + remaining * release_rate;
    released += release_rate;
    if (cooldown_human > 0) {
        //std::cout << "On cooldown" << std::endl;
        return elephant_moving(actual_human, actual_elephant, cooldown_human - 1, cooldown_elephant, important_valves,
                               targetted, opened, flow_rate, all_distances, release_rate, released, remaining,
                               not_opened, precedent_choice);
    }
    if (!opened[actual_human]) {
        //std::cout << "Opened " << actual_human << std::endl;
        opened[actual_human] = true;
        return elephant_moving(actual_human, actual_elephant, cooldown_human, cooldown_elephant, important_valves, targetted, opened, flow_rate, all_distances, release_rate + flow_rate[actual_human], released, remaining, not_opened - 1, precedent_choice);
    }
    uint64_t max = 0;
    for (const auto &valve: important_valves) {
        if (valve == actual_human || targetted[valve] || all_distances.at(actual_human).at(valve) - 1 >= remaining)
            continue;
        targetted[valve] = true;
        precedent_choice.push_back(valve);
        max = std::max(max, elephant_moving(valve, actual_elephant, all_distances.at(actual_human).at(valve) - 1, cooldown_elephant, important_valves, targetted, opened, flow_rate, all_distances, release_rate, released, remaining, not_opened, precedent_choice));
        targetted[valve] = false;
        precedent_choice.pop_back();
    }
    if (max == 0)
        max = elephant_moving(actual_human, actual_elephant, cooldown_human, cooldown_elephant, important_valves, targetted, opened, flow_rate, all_distances, release_rate, released, remaining, not_opened, precedent_choice);
    return (max);
}

int first(const std::map<std::string, uint64_t> &str_to_id, const std::map<uint64_t, std::string> &id_to_str, const std::vector<uint64_t> &flow_rate, const std::vector<std::vector<uint64_t>> &adjacents)
{
    uint64_t starting_point = str_to_id.at("AA");
    std::vector<uint64_t> important_valves = get_important_valves(flow_rate, starting_point);
    std::map<uint64_t,bool> opened = create_opened_map(important_valves, starting_point);
    std::map<uint64_t,std::map<uint64_t,uint64_t>> all_distances = calculate_all_distances(important_valves, adjacents, id_to_str);

    for (const auto &[valve, valves]: all_distances) {
        std::cout << "Distance from " << id_to_str.at(valve) << " to " << std::endl;
        for (const auto &[arrivalve, distance]: valves) {
            std::cout << "    " << id_to_str.at(arrivalve) << " is " << distance << std::endl;
        }
    }
    std::vector<uint64_t> precedent_choice;
    uint64_t result = bruteforce_result(starting_point, important_valves, opened, flow_rate, all_distances, 0, 0, 0, 30, opened.size() - 1, precedent_choice);
    std::cout << result << std::endl;
}

int second(const std::map<std::string, uint64_t> &str_to_id, const std::map<uint64_t, std::string> &id_to_str, const std::vector<uint64_t> &flow_rate, const std::vector<std::vector<uint64_t>> &adjacents)
{
    uint64_t starting_point = str_to_id.at("AA");
    std::vector<uint64_t> important_valves = get_important_valves(flow_rate, starting_point);
    std::map<uint64_t,bool> opened = create_opened_map(important_valves, starting_point);
    std::map<uint64_t,bool> chosen = create_opened_map(important_valves, starting_point);
    std::map<uint64_t,std::map<uint64_t,uint64_t>> all_distances = calculate_all_distances(important_valves, adjacents, id_to_str);

    for (const auto &[valve, valves]: all_distances) {
        std::cout << "Distance from " << id_to_str.at(valve) << " to " << std::endl;
        for (const auto &[arrivalve, distance]: valves) {
            std::cout << "    " << id_to_str.at(arrivalve) << " is " << distance << std::endl;
        }
    }
    std::vector<uint64_t> precedent_choice;
    uint64_t result = human_moving(starting_point, starting_point, 0, 0, important_valves, chosen, opened, flow_rate, all_distances, 0, 0, 26, opened.size() - 1, precedent_choice);
    std::cout << result << std::endl;
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_16/input");
    std::regex regex("Valve (\\w+) has flow rate=(\\d+);(?:(?: tunnels lead to valves )|(?: tunnel leads to valve ))(.*)");
    std::smatch matches;
    std::vector<std::vector<std::string>> adjacents_str;
    std::map<std::string, uint64_t> str_to_id;
    std::map<uint64_t, std::string> id_to_str;
    std::vector<uint64_t> flow_rate;
    std::vector<std::vector<uint64_t>> adjacents;
    uint64_t id = 0;

    for (const auto &line: file_content) {
        if (std::regex_search(line, matches, regex)) {
            str_to_id.emplace(matches[1], id);
            id_to_str.emplace(id++, matches[1]);
            flow_rate.push_back(std::stoul(matches[2]));
            adjacents_str.push_back(split(matches[3], ", "));
        }
    }
    for (const auto &adjacent_str: adjacents_str) {
        std::vector<uint64_t> adjacent;
        for (const auto &str: adjacent_str) {
            adjacent.push_back(str_to_id[str]);
        }
        adjacents.push_back(adjacent);
    }
    //first(str_to_id, id_to_str, flow_rate, adjacents);
    second(str_to_id, id_to_str, flow_rate, adjacents);
    return (0);
}
