#include "presentation.hpp"

#include <iostream>

#include "structs.hpp"

namespace {

void EnsureValueExists(size_t i, const std::vector<std::string>& tokens,
                       const std::string& arg) {
  if (i + 1 >= tokens.size()) {
    throw std::runtime_error("Missing value for " + arg);
  }

  if (!tokens[i + 1].empty() && tokens[i + 1][0] == '-') {
    throw std::runtime_error("Missing value for " + arg);
  }
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

namespace {

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

  std::cout << "Тип транспорта: " << transport_types.map[segment.transport_type]
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

std::vector<std::string> CLIParser::Tokenize(const std::string& line) {
  std::istringstream input(line);
  std::vector<std::string> tokens;
  std::string token;

  while (input >> token) {
    tokens.push_back(token);
  }

  return tokens;
}

RouteRequest CLIParser::ParseTokens(const std::vector<std::string>& tokens) {
  RouteRequest request{};

  for (size_t i = 0; i < tokens.size(); ++i) {
    const std::string& arg = tokens[i];

    if (arg == "--from") {
      EnsureValueExists(i, tokens, arg);
      request.departure_city_ = tokens[++i];
    } else if (arg == "--to") {
      EnsureValueExists(i, tokens, arg);
      request.arrival_city_ = tokens[++i];
    } else if (arg == "--date") {
      EnsureValueExists(i, tokens, arg);
      request.date_ = tokens[++i];
    } else if (arg == "--transport") {
      EnsureValueExists(i, tokens, arg);
      request.transport_type_ = tokens[++i];
    } else if (arg == "--transfers") {
      request.has_transfers_ = tokens[i];
    } else {
      throw std::runtime_error("Unknown argument: " + arg);
    }
  }

  if (request.departure_city_.empty()) {
    throw std::runtime_error("Missing --from");
  }
  if (request.arrival_city_.empty()) {
    throw std::runtime_error("Missing --to");
  }
  if (request.date_.empty()) {
    throw std::runtime_error("Missing --date");
  }

  return request;
}

void CLIParser::PrintHelp() {}