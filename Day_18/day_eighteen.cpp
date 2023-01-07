#include <iostream>
#include <vector>
#include <tuple>
#include <regex>

std::vector<std::string> get_file_content(const std::string &filepath);

int64_t count_exposed_faces(const std::tuple<int64_t,int64_t,int64_t> &coordinate, const std::vector<std::vector<std::vector<int64_t>>> &array)
{
    int64_t total = 0;
    const auto &[x, y ,z] = coordinate;

    if (x == array.size() - 1 || array[x + 1][y][z] == 0)
        total++;
    if (x == 0 || array[x - 1][y][z] == 0)
        total++;
    if (y == array.size() - 1 || array[x][y + 1][z] == 0)
        total++;
    if (y == 0 || array[x][y - 1][z] == 0)
        total++;
    if (z == array.size() - 1 || array[x][y][z + 1] == 0)
        total++;
    if (z == 0 || array[x][y][z - 1] == 0)
        total++;
    return (total);
}

void add_to_map(const std::tuple<int64_t,int64_t,int64_t> &coordinate, std::map<std::tuple<int64_t,int64_t,int64_t>,int64_t> &map)
{
    auto insertion_result = map.insert(std::pair(coordinate, 1));
    if (!insertion_result.second) {
        map[coordinate] += 1;
    }
}

void add_exposed_block_to_map(const std::tuple<int64_t,int64_t,int64_t> &coordinate, const std::vector<std::vector<std::vector<int64_t>>> &array, std::map<std::tuple<int64_t,int64_t,int64_t>,int64_t> &map)
{
    const auto &[x, y ,z] = coordinate;

    if (x == array.size() - 1 || array[x + 1][y][z] == 0)
        add_to_map(std::tuple(x + 1, y, z), map);
    if (x == 0 || array[x - 1][y][z] == 0)
        add_to_map(std::tuple(x - 1, y, z), map);
    if (y == array.size() - 1 || array[x][y + 1][z] == 0)
        add_to_map(std::tuple(x, y + 1, z), map);
    if (y == 0 || array[x][y - 1][z] == 0)
        add_to_map(std::tuple(x, y - 1, z), map);
    if (z == array.size() - 1 || array[x][y][z + 1] == 0)
        add_to_map(std::tuple(x, y, z + 1), map);
    if (z == 0 || array[x][y][z - 1] == 0)
        add_to_map(std::tuple(x, y, z - 1), map);
}

int first(const std::vector<std::tuple<int64_t,int64_t,int64_t>> &coordinates, const std::vector<std::vector<std::vector<int64_t>>> &array)
{
    std::map<std::tuple<int64_t,int64_t,int64_t>,int64_t> map;
    int64_t total = 0;

    for (const auto &coordinate: coordinates) {
        add_exposed_block_to_map(coordinate, array, map);
    }
    for (const auto &pair: map)
        total += pair.second;
    std::cout << total << std::endl;
}

void set_place_to_watered(const std::vector<std::tuple<int64_t,int64_t,int64_t>> &coordinates_to_check, std::vector<std::vector<std::vector<int64_t>>> &array)
{
    for (const auto &[x,y,z]: coordinates_to_check) {
        array[x][y][z] = 2;
    }
}

void check_x(const std::tuple<int64_t,int64_t,int64_t> &coordinate, std::vector<std::tuple<int64_t,int64_t,int64_t>> &next_coordinate_to_check, std::vector<std::vector<std::vector<int64_t>>> &array, uint64_t &total)
{
    const auto &[x, y, z] = coordinate;

    if (x != 0) {
        if (array[x - 1][y][z] == 1)
            total++;
        if (array[x - 1][y][z] == 0) {
            next_coordinate_to_check.emplace_back(x - 1, y, z);
            array[x - 1][y][z] = 2;
        }
    }
    if (x != array.size() - 1) {
        if (array[x + 1][y][z] == 1)
            total++;
        if (array[x + 1][y][z] == 0) {
            next_coordinate_to_check.emplace_back(x + 1, y, z);
            array[x + 1][y][z] = 2;
        }
    }
}

void check_y(const std::tuple<int64_t,int64_t,int64_t> &coordinate, std::vector<std::tuple<int64_t,int64_t,int64_t>> &next_coordinate_to_check, std::vector<std::vector<std::vector<int64_t>>> &array, uint64_t &total)
{
    const auto &[x, y, z] = coordinate;

    if (y != 0) {
        if (array[x][y - 1][z] == 1)
            total++;
        if (array[x][y - 1][z] == 0) {
            next_coordinate_to_check.emplace_back(x, y - 1, z);
            array[x][y - 1][z] = 2;
        }
    }
    if (y != array.size() - 1) {
        if (array[x][y + 1][z] == 1)
            total++;
        if (array[x][y + 1][z] == 0) {
            next_coordinate_to_check.emplace_back(x, y + 1, z);
            array[x][y + 1][z] = 2;
        }
    }
}

void check_z(const std::tuple<int64_t,int64_t,int64_t> &coordinate, std::vector<std::tuple<int64_t,int64_t,int64_t>> &next_coordinate_to_check, std::vector<std::vector<std::vector<int64_t>>> &array, uint64_t &total)
{
    const auto &[x, y, z] = coordinate;

    if (z != 0) {
        if (array[x][y][z - 1] == 1)
            total++;
        if (array[x][y][z - 1] == 0) {
            next_coordinate_to_check.emplace_back(x, y, z - 1);
            array[x][y][z - 1] = 2;
        }
    }
    if (z != array.size() - 1) {
        if (array[x][y][z + 1] == 1)
            total++;
        if (array[x][y][z + 1] == 0) {
            next_coordinate_to_check.emplace_back(x, y, z + 1);
            array[x][y][z + 1] = 2;
        }
    }
}

void flood_fill(const std::vector<std::tuple<int64_t,int64_t,int64_t>> &coordinates_to_check, std::vector<std::vector<std::vector<int64_t>>> &array, uint64_t &total)
{
    std::vector<std::tuple<int64_t,int64_t,int64_t>> next_coordinate_to_check;
    std::cout << "NEW ENTRY" << std::endl;

    if (coordinates_to_check.empty())
        return;
    set_place_to_watered(coordinates_to_check, array);
    for (const auto &coordinate: coordinates_to_check) {
        std::cout << std::get<0>(coordinate) << "," << std::get<1>(coordinate) << "," << std::get<2>(coordinate) << std::endl;
        check_x(coordinate, next_coordinate_to_check, array, total);
        check_y(coordinate, next_coordinate_to_check, array, total);
        check_z(coordinate, next_coordinate_to_check, array, total);
    }
    flood_fill(next_coordinate_to_check, array, total);
}

int second(std::vector<std::vector<std::vector<int64_t>>> array)
{
    uint64_t total = 0;
    std::vector<std::tuple<int64_t,int64_t,int64_t>> coordinates_to_check(1, std::make_tuple(0,0,0));
    flood_fill(coordinates_to_check, array, total);
    std::cout << total << std::endl;
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_18/input");
    std::vector<std::tuple<int64_t,int64_t,int64_t>> coordinates;
    std::regex regex("(\\d+),(\\d+),(\\d+)");
    std::smatch matches;
    std::int64_t size = 0;
    for (const auto &line: file_content) {
        if (std::regex_search(line, matches, regex)) {
            int64_t x = std::stol(matches[1]);
            int64_t y = std::stol(matches[2]);
            int64_t z = std::stol(matches[3]);
            int64_t max = std::max(std::max(x, y), z);
            size = std::max(size, max);
            coordinates.emplace_back(x, y, z);
        }
    }
    std::vector<std::vector<std::vector<int64_t>>> array = std::vector(size + 3, std::vector(size + 3, std::vector<int64_t>(size + 3, 0)));

    for (const auto &[x, y, z]: coordinates) {
        array[x + 1][y + 1][z + 1] = 1;
    }
    first(coordinates, array);
    second(array);
    return (0);
}
