#include <iostream>
#include <vector>
#include <cmath>

std::vector<std::string> get_file_content(const std::string &filepath);

int64_t SNAFU_digit_to_long(const char &c)
{
    if (c == '2') return (2);
    if (c == '1') return (1);
    if (c == '0') return (0);
    if (c == '-') return (-1);
    if (c == '=') return (-2);
    return (0);
}

int64_t SNAFU_to_long(const std::string &SNAFU)
{
    int64_t number = 0;

    for (size_t i = 0; i < SNAFU.size(); i++) {
        number += SNAFU_digit_to_long(SNAFU[i]) * (int64_t)std::pow(5, SNAFU.size() - i - 1);
    }
    return (number);
}

std::string long_to_SNAFU(int64_t number)
{
    // NUMBER FROM BASE 10 TO BASE 5
    // FROM BASE 5 NUMBER STRING
    // IF VALUE == 5, 4 or 3 next dizaine += 1 et actual equal respectively 0, -, =
}

int first(const std::vector<std::string> &SNAFU_numbers)
{
    int64_t total = 0;

    for (const auto &number: SNAFU_numbers) {
        total += SNAFU_to_long(number);
    }
    std::cout << total << std::endl;
    return (0);
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_25/input");
    first(file_content);
    std::cout << SNAFU_to_long("2-0-01==0-1=2212=100") << std::endl;
    return (0);
}
