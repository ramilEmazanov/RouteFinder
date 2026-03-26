#include "presentation.hpp"

#include <iostream>

#include "structs.hpp"

namespace {

std::unordered_map<std::string, std::string> kTransportNames{
    {"plane", "Самолет"},           {"train", "Поезд"},
    {"suburban", "Электричка"},     {"bus", "Автобус"},
    {"water", "Морской транспорт"}, {"helicopter", "Вертолет"}};

std::unordered_set<std::string> kAllowed = {"train",    "plane", "bus",
                                            "suburban", "water", "helicopter"};

bool IsValidDateFormat(const std::string& date) {
  if (date.size() != 10) {
    return false;
  }
  return std::isdigit(date[0]) && std::isdigit(date[1]) &&
         std::isdigit(date[2]) && std::isdigit(date[3]) && date[4] == '-' &&
         std::isdigit(date[5]) && std::isdigit(date[6]) && date[7] == '-' &&
         std::isdigit(date[8]) && std::isdigit(date[9]);
}

std::string SecondsToHoursMinutes(int seconds) {
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;

  std::string result;

  if (hours > 0) {
    result += std::to_string(hours) + " ч ";
  }

  if (minutes > 0 || hours == 0) {
    result += std::to_string(minutes) + " мин";
  }

  return result;
}

std::string FormatDateTime(const std::string& iso) {
  if (iso.size() < 25) {
    return iso;
  }

  std::string day = iso.substr(8, 2);
  std::string month = iso.substr(5, 2);
  std::string time = iso.substr(11, 5);
  std::string offset = iso.substr(19, 6);

  return day + "." + month + " " + time + " (" + "UTC " + offset + ")";
}

}  // namespace

void RoutePrinter::PrintSegment(const Segment& segment, int index) {
  std::cout << "Сегмент №" << index << ":\n";
  std::cout << "Город отправления: " << segment.departure_city_title << '\n';
  std::cout << "Станция отправления: " << segment.departure_station_title
            << '\n';
  std::cout << "Время отправления: " << FormatDateTime(segment.departure_time)
            << '\n';

  std::cout << "Город прибытия: " << segment.arrival_city_title << '\n';
  std::cout << "Станция прибытия: " << segment.arrival_station_title << '\n';
  std::cout << "Время прибытия: " << FormatDateTime(segment.arrival_time)
            << '\n';

  std::cout << "Тип транспорта: " << kTransportNames[segment.transport_type]
            << '\n';

  std::cout << "Перевозчик / маршрут: " << segment.carrier_title << '\n';
  std::cout << "Длительность: "
            << SecondsToHoursMinutes(segment.duration_seconds) << '\n';
}

void RoutePrinter::PrintTransferInfo(const TransferInfo& transfer) {
  std::cout << "Пересадка:\n";
  std::cout << "Город пересадки: " << transfer.city_title << '\n';
  std::cout << "Станция пересадки (прибытие): "
            << transfer.transfer_from_station_title << '\n';
  std::cout << "Станция пересадки (отправление): "
            << transfer.transfer_to_station_title << '\n';
  std::cout << "Длительность пересадки: "
            << SecondsToHoursMinutes(transfer.duration_seconds) << '\n';
}

void RoutePrinter::PrintRoutes(
    const std::optional<std::vector<Route>>& routes) {
  if (!routes.has_value()) {
    std::cout << "No routes found\n";
    return;
  }

  if (routes->empty()) {
    std::cout << "No routes found\n";
    return;
  }

  for (size_t i = 0; i < routes->size(); ++i) {
    const Route& route = (*routes)[i];

    std::cout << "Путь №" << i + 1 << ":\n";
    std::cout << "Город отправления: " << route.departure_city_title << '\n';
    std::cout << "Город прибытия: " << route.arrival_city_title << '\n';
    std::cout << "Дата: " << route.date_ << '\n';

    if (!route.transfer_info.has_value()) {
      std::cout << "Без пересадок\n";
    } else {
      std::cout << "Количество пересадок: 1\n";
    }

    std::cout << '\n';

    for (size_t j = 0; j < route.segments.size(); ++j) {
      PrintSegment(route.segments[j], static_cast<int>(j + 1));
      std::cout << '\n';

      if (j == 0 && route.transfer_info.has_value()) {
        PrintTransferInfo(*route.transfer_info);
        std::cout << '\n';
      }
    }

    std::cout << "----------------------------------------\n";
  }
}

std::string CLIParser::AskNonEmptyString(const std::string& request) {
  while (true) {
    std::cout << request;
    std::string input;
    std::getline(std::cin, input);

    if (!input.empty()) {
      return input;
    }

    std::cout << "Поле не может быть пустым. Попробуйте ещё раз.\n";
  }
}

std::string CLIParser::AskDate() {
  while (true) {
    std::cout << "Введите дату (YYYY-MM-DD): ";
    std::string date;
    std::getline(std::cin, date);

    if (IsValidDateFormat(date)) {
      return date;
    }

    std::cout << "Некорректный формат даты. Попробуйте ещё раз.\n";
  }
}

std::optional<std::string> CLIParser::AskTransport() {
  while (true) {
    std::cout << "Введите тип транспорта "
                 "(train/plane/bus/suburban/water/helicopter) "
                 "или оставьте пустым: ";

    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) {
      return std::nullopt;
    }

    if (kAllowed.contains(input)) {
      return input;
    }

    std::cout << "Неизвестный тип транспорта. Попробуйте ещё раз.\n";
  }
}

std::string CLIParser::AskTransfers() {
  while (true) {
    std::cout << "Разрешить до одной пересадки? (yes/no): ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "yes") {
      return "true";
    }
    if (input == "no") {
      return "false";
    }

    std::cout << "Введите yes или no.\n";
  }
}

RouteRequest CLIParser::ReadInteractiveRequest() {
  RouteRequest request;
  request.departure_city_ = AskNonEmptyString("Введите город отправления: ");
  request.arrival_city_ = AskNonEmptyString("Введите город прибытия: ");
  request.date_ = AskDate();
  request.transport_type_ = AskTransport();
  request.has_transfers_ = AskTransfers();
  return request;
}

void CLIParser::PrintHelp() {
  std::cout
      << "Программа ищет маршруты между городами через API Яндекс Расписаний.\n"
         "В интерактивном режиме поля вводятся по отдельности.\n";
}