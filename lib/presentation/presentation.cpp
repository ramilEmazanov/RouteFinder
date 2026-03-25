#include "presentation.hpp"

void CLIParser::EnsureValueExists(int i, int argc, const std::string& arg) {
  if (i + 1 >= argc) {
    throw std::runtime_error("Missing value for " + arg);
  }
  std::vector<std::string> flags = {"--from", "--to", "--date", "--transport"};

  for (const auto& flag : flags) {
    if (flag == arg) {
      throw std::runtime_error("Missing value for " + arg);
    }
  }
}

RouteRequest CLIParser::ParseCLI(int argc, const char** argv) {
  RouteRequest request{};

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "--from") {
      EnsureValueExists(i, argc, argv[i + 1]);
      request.departure_city_ = argv[++i];
    }
    if (arg == "--to") {
      EnsureValueExists(i, argc, argv[i + 1]);
      request.arrival_city_ = argv[++i];
    }
    if (arg == "--date") {
      EnsureValueExists(i, argc, argv[i + 1]);
      request.date_ = argv[++i];
    }
    if (arg == "--transport") {
      EnsureValueExists(i, argc, argv[i + 1]);
      request.transport_type_ = argv[++i];
    }
    if (arg == "--transfers") {
      request.has_transfers_ = "true";
    }
  }
  if (request.departure_city_.empty() || request.arrival_city_.empty() ||
      request.date_.empty()) {
    throw std::runtime_error("No required argument");
  }

  return request;
}

#include <iostream>

#include "structs.hpp"

namespace {

void PrintSegment(const Segment& segment, int index) {
  std::cout << "Сегмент №" << index << ":\n";
  std::cout << "Город отправления: " << segment.departure_city_title << '\n';
  std::cout << "Станция отправления: " << segment.departure_station_title
            << '\n';
  std::cout << "Время отправления: " << segment.departure_time << '\n';

  std::cout << "Город прибытия: " << segment.arrival_city_title << '\n';
  std::cout << "Станция прибытия: " << segment.arrival_station_title << '\n';
  std::cout << "Время прибытия: " << segment.arrival_time << '\n';

  std::cout << "Тип транспорта: " << segment.transport_type << '\n';
  std::cout << "Перевозчик / маршрут: " << segment.carrier_title << '\n';
  std::cout << "Длительность: " << segment.duration_seconds << " секунд\n";
}

void PrintTransferInfo(const TransferInfo& transfer) {
  std::cout << "Пересадка:\n";
  std::cout << "Город пересадки: " << transfer.city_title << '\n';
  std::cout << "Станция пересадки (прибытие): "
            << transfer.transfer_from_station_title << '\n';
  std::cout << "Станция пересадки (отправление): "
            << transfer.transfer_to_station_title << '\n';
  std::cout << "Длительность пересадки: " << transfer.duration_seconds
            << " секунд\n";
}

}  // namespace

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