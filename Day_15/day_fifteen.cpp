#include <iostream>
#include <vector>
#include <regex>
#include <set>

std::vector<std::string> get_file_content(const std::string &filepath);

int64_t calculate_distance(std::pair<int64_t,int64_t> sensor, std::pair<int64_t,int64_t> beacon)
{
    int64_t distance = std::abs(sensor.first - beacon.first) + std::abs(sensor.second - beacon.second);
    return (distance);
}

void get_beacon_free_spaces(const std::pair<int64_t,int64_t> &sensor, const std::pair<int64_t,int64_t> &beacon, std::set<uint64_t> &free_spaces, int64_t distance, int64_t y)
{
    if (y > sensor.second + distance || y < sensor.second - distance)
        return;
    for (int64_t x = sensor.first - ((distance - calculate_distance(sensor, {sensor.first, y})) * 2 + 1) / 2; x <= sensor.first + ((distance - calculate_distance(sensor, {sensor.first, y})) * 2 + 1) / 2; x++) {
        if (std::make_pair(x, y) == beacon)
            continue;
        free_spaces.emplace(x);
    }
}

std::pair<uint64_t ,std::vector<std::pair<int64_t,int64_t>>> determine_square_info(const std::pair<int64_t,int64_t> &sensor, int64_t distance)
{
    std::pair<uint64_t ,std::vector<std::pair<int64_t,int64_t>>> square_info;

    square_info.first = distance;
    square_info.second.emplace_back(sensor.first, sensor.second - distance);
    square_info.second.emplace_back(sensor.first, sensor.second + distance);
    square_info.second.emplace_back(sensor.first - distance, sensor.second);
    square_info.second.emplace_back(sensor.first - distance, sensor.second);
    return (square_info);
}


/*int first(std::vector<std::pair<int64_t,int64_t>> sensors, std::vector<std::pair<int64_t,int64_t>> beacons)
{
    std::vector<std::pair<uint64_t ,std::vector<std::pair<int64_t,int64_t>>>> squares;
    for (size_t i = 0; i < sensors.size(); i++) {
        std::cout << "Sensor " << i + 1 << "/" << sensors.size() << std::endl;
        int64_t distance = calculate_distance(sensors[i], beacons[i]);
        std::pair<uint64_t ,std::vector<std::pair<int64_t,int64_t>>> square = determine_square_info(sensors[i], distance);
        squares.push_back(square);
    }
    std::cout << all_beacon_free_spaces[2000000].size() << std::endl;
    return (0);
}*/

int first(std::vector<std::pair<int64_t,int64_t>> sensors, std::vector<std::pair<int64_t,int64_t>> beacons)
{
    std::set<uint64_t> free_spaces;
    for (size_t i = 0; i < sensors.size(); i++) {
        int64_t distance = calculate_distance(sensors[i], beacons[i]);
        get_beacon_free_spaces(sensors[i], beacons[i], free_spaces, distance, 2000000);
    }
    std::cout << free_spaces.size() << std::endl;
    return (0);
}

void rearrage_vector_according_to_pair(std::vector<std::pair<int64_t,int64_t>> &ranges, std::pair<int64_t,int64_t> new_range)
{
    std::vector<std::pair<int64_t,int64_t>> ranges_copy = ranges;
    size_t nb_popped = 0;
    for (size_t i = 0; i < ranges_copy.size(); i++) {
        //Premier intérieur
        if (new_range.first >= ranges_copy[i].first && new_range.first < ranges_copy[i].second && new_range.second > ranges_copy[i].second)
            ranges[i - nb_popped].second = new_range.first;
        //Deuxième intérieur
        if (new_range.first < ranges_copy[i].first && new_range.second > ranges_copy[i].first && new_range.second <= ranges_copy[i].second)
            ranges[i - nb_popped].first = new_range.second;
        //Les deux intérieurs
        if (new_range.first >= ranges_copy[i].first && new_range.second <= ranges_copy[i].second) {
            ranges.emplace_back(new_range.second, ranges_copy[i].second);
            ranges[i - nb_popped].second = new_range.first;
        }
        //Les deux extérieurs
        if (new_range.first < ranges_copy[i].first && new_range.second > ranges_copy[i].second) {
            ranges.erase(ranges.begin() + (i - nb_popped));
            nb_popped++;
        }
    }
}

void update_possible_ranges(std::pair<int64_t,int64_t> sensor, std::pair<int64_t,int64_t>beacon, std::vector<std::vector<std::pair<int64_t,int64_t>>> &possibles_ranges, int64_t distance, int64_t max_y)
{
    if (sensor.second - distance > max_y || sensor.second + distance < 0) {
        std::cout << "Return early" << std::endl;
        return;
    }
    for (int64_t y = sensor.second - distance; y <= sensor.second + distance; y++) {
        //std::cout << "Loop : " << y << std::endl;
        if (y < 0) {
            y = -1;
            //std::cout << "Continue : " << y << std::endl;
            continue;
        }
        if (y > max_y) {
            //std::cout << "Break : " << y << std::endl;
            break;
        }
        std::pair<int64_t,int64_t> new_range = {
            (sensor.first - ((distance - calculate_distance(sensor, {sensor.first, y})) * 2 + 1) / 2) - 1,
            (sensor.first + ((distance - calculate_distance(sensor, {sensor.first, y})) * 2 + 1) / 2) + 1
        };
        //std::cout << y << ": " << new_range.first << " " << new_range.second << std::endl;
        rearrage_vector_according_to_pair(possibles_ranges[y], new_range);
        //std::cout << "AFTER REARANGE" << std::endl;
        /*for (const auto &range: possibles_ranges[y]) {
            std::cout << range.first << " " << range.second << std::endl;
        }*/
    }
}

int second(std::vector<std::pair<int64_t,int64_t>> sensors, std::vector<std::pair<int64_t,int64_t>> beacons, int64_t max_y)
{
    std::vector<std::vector<std::pair<int64_t,int64_t>>> possible_ranges = std::vector(max_y + 1, std::vector<std::pair<int64_t,int64_t>>(1, {0, max_y}));
    for (size_t i = 0; i < sensors.size(); i++) {
        int64_t distance = calculate_distance(sensors[i], beacons[i]);
        std::cout << "Sensor number " << i << " with distance " << distance << std::endl;
        update_possible_ranges(sensors[i], beacons[i], possible_ranges, distance, max_y);
    }
    std::cout << "END" << std::endl;
    for (size_t i = 0; i < possible_ranges.size(); i++) {
        for (const auto &range: possible_ranges[i]) {
            std::cout << i << " " <<range.first << " " << range.second << std::endl;
        }
    }
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_15/input");
    std::regex regex("Sensor at x=(-?\\d+), y=(-?\\d+): closest beacon is at x=(-?\\d+), y=(-?\\d+)");
    std::smatch matches;
    std::vector<std::pair<int64_t,int64_t>> sensors;
    std::vector<std::pair<int64_t,int64_t>> beacons;

    for (const auto &line: file_content) {
        if(std::regex_search(line, matches, regex)) {
            std::pair sensor = {std::stol(matches[1]), std::stol(matches[2])};
            std::pair beacon = {std::stol(matches[3]), std::stol(matches[4])};
            sensors.push_back(sensor);
            beacons.push_back(beacon);
        }
    }
    //first(sensors, beacons);
    second(sensors, beacons, 4000000);
    return (0);
}
