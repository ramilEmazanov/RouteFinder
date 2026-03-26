#include "business_logic.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "structs.hpp"

bool MainService::RouteFilter(const Route& route, const RouteRequest& request) {
  if (request.has_transfers_ == "false" && route.transfer_info.has_value()) {
    return false;
  }

  if (request.transport_type_.has_value()) {
    for (const auto& segment : route.segments) {
      if (segment.transport_type != request.transport_type_) {
        return false;
      }
    }
  }

  return true;
}

MainService::MainService(IRouteRepository& repo, ICitiesDict& cities)
    : repo_(repo), cities_(cities) {
  cities_dict = cities_.MakeCitiesDict();
}

std::optional<std::vector<Route>> MainService::ProcessRequest(
    const RouteRequest& request) {
  if (!cities_dict.contains(request.arrival_city_) ||
      !cities_dict.contains(request.departure_city_)) {
    throw std::runtime_error("No requested cities found in database\n");
  }

  RouteRequest updated_request(request);
  updated_request.departure_city_ = cities_dict[request.departure_city_];
  updated_request.arrival_city_ = cities_dict[request.arrival_city_];

  std::optional<std::vector<Route>> response = repo_.GetRoute(updated_request);

  if (!response.has_value()) {
    return std::nullopt;
  }

  std::vector<Route> filtered_results;
  filtered_results.reserve(response.value().size());

  for (const auto& route : response.value()) {
    if (RouteFilter(route, request)) {
      filtered_results.push_back(route);
    }
  }

  return filtered_results;
}
