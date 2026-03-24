#include "business_logic.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "structs.hpp"

MainService::MainService(IRouteRepository& repo, ICitiesDict& cities)
    : repo_(repo), cities_(cities) {}

std::optional<std::vector<Route>> MainService::ProcessRequest(const RouteRequest& request) {
  std::unordered_map<std::string, std::string> cities_dict =
      cities_.MakeCitiesDict();

  if (!cities_dict.contains(request.arrival_city_) ||
      !cities_dict.contains(request.departure_city_)) {
    throw std::runtime_error("No requested cities found\n");
  }

  // Распарсить дату, перевести в unix_time, проверить, что она старше
  // вчерашнего дня

  RouteRequest updated_request(request);
  updated_request.departure_city_ = cities_dict[request.departure_city_];
  updated_request.arrival_city_ = cities_dict[request.arrival_city_];

  std::optional<std::vector<Route>> response = repo_.GetRoute(updated_request);

  if(!response.has_value()) {
    return std::nullopt;
  }

  std::vector<Route> filtered_results;
  filtered_results.reserve(response.value().size());

  
  // Фильтруем нужные нам данные, отдаем в RoutePrint

  return response.value();
  // Получает vector<Route> без учета фильтров по количеству пересадок, типу
  // транспорта Он сам будет фильтровать это
}
