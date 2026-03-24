#include "json_work.hpp"

#include <iostream>
// Служебный класс для работы с Json
// Достает из переданного ему JSON кэша и Request - vector<Route>
std::vector<Route> CacheJsonExtracter::Extract(const json& data,
                                               const RouteRequest& request) {
  std::vector<Route> route;
  return route;
}

// Служебный класс для работы с JSON
// Достает из переданного ему JSON из api vector<Route>
std::optional<std::vector<Route>> APIJsonExtracter::Extract(const json& data,
                                             const RouteRequest& request) {
  int routes_count = data["pagination"]["total"].get<int>();
  if(routes_count == 0) {
    return std::nullopt;
  }
  
  std::vector<Route> response;
  response.reserve(routes_count);

  for (const auto& segment : data["segments"]) {
    if (segment["has_transfers"] == false) {
      Route route;
      route.departure_city_ = segment["from"]["title"];
      route.departure_time_ = segment["departure"];
      
      route.arrival_city_ = segment["to"]["title"];
      route.arrival_time_ = segment["arrival"];
      response.push_back(route);
    }
  }

  if(response.empty()) {
    return std::nullopt;
  }

  return response;
}

// Служебный класс для работы с JSON
void JsonWriter::Write(const RouteRequest& request,
                       const std::vector<Route>& route, json& data) {
  // Записывать в json в конец преобразованный route и выдавать тот же самый
  // json?
}
