#include <array>
#include <iostream>
#include <string_view>

namespace {

bool IsValidDate(std::string_view date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }
    for (std::size_t i = 0; i < date.size(); ++i) {
        if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9')) {
            return false;
        }
    }

    const int year = (date[0] - '0') * 1000 + (date[1] - '0') * 100
        + (date[2] - '0') * 10 + date[3] - '0';
    const int month = (date[5] - '0') * 10 + date[6] - '0';
    const int day = (date[8] - '0') * 10 + date[9] - '0';
    if (year == 0 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    constexpr std::array<int, 12> kDaysInMonth{31, 28, 31, 30, 31, 30,
                                               31, 31, 30, 31, 30, 31};
    const bool leap_year = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    const int max_day = kDaysInMonth[month - 1] + (month == 2 && leap_year ? 1 : 0);
    return day <= max_day;
}

void PrintHelp(std::ostream& output) {
    output << "Использование:\n"
           << "  route_finder --validate-date YYYY-MM-DD  Проверить дату поездки\n"
           << "  route_finder --help                      Показать справку\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc == 2 && (std::string_view(argv[1]) == "--help"
                      || std::string_view(argv[1]) == "-h")) {
        PrintHelp(std::cout);
        return 0;
    }
    if (argc != 3 || std::string_view(argv[1]) != "--validate-date") {
        PrintHelp(std::cerr);
        return 1;
    }
    if (!IsValidDate(argv[2])) {
        std::cerr << "Ошибка: укажите существующую дату в формате YYYY-MM-DD "
                     "(год от 0001 до 9999).\n";
        return 1;
    }
    std::cout << "Дата корректна: " << argv[2] << '\n';
    return 0;
}
