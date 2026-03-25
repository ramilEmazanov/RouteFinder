#include "json_work.hpp"

#include <iostream>
// Служебный класс для работы с Json
// Достает из переданного ему JSON кэша и Request - vector<Route>
std::vector<Route> CacheJsonExtracter::Extract(const json& data,
                                               const RouteRequest& request) {
  std::vector<Route> route;
  return route;
}

Route APIJsonExtracter::ParseDirectRoute(const json& data,
                                         const json& segment) {
  Route route;
  route.departure_city_title = data["search"]["from"]["title"];
  route.arrival_city_title = data["search"]["to"]["title"];
  route.date_ = data["search"]["date"];

  Segment route_segment;
  route_segment.departure_city_title = route.departure_city_title;
  route_segment.departure_station_title = segment["from"]["title"];
  route_segment.departure_time = segment["departure"];

  route_segment.arrival_city_title = route.arrival_city_title;
  route_segment.arrival_station_title = segment["to"]["title"];
  route_segment.arrival_time = segment["arrival"];

  route_segment.transport_type = segment["thread"]["transport_type"];
  route_segment.carrier_title = segment["thread"]["carrier"]["title"];

  route_segment.duration_seconds =
      static_cast<int>(segment["duration"].get<double>());

  route.segments.push_back(route_segment);
  return route;
}

Route APIJsonExtracter::ParseOneTransferRoute(const json& data,
                                              const json& segment) {
  Route route;
  route.departure_city_title = data["search"]["from"]["title"];
  route.arrival_city_title = data["search"]["to"]["title"];
  route.date_ = data["search"]["date"];

  const auto& details = segment["details"];

  for (const auto& item : details) {
    if (item.contains("is_transfer") && item["is_transfer"] == true) {
      TransferInfo transfer;
      transfer.city_title = item["transfer_point"]["title"];
      transfer.transfer_from_station_title = item["transfer_from"]["title"];
      transfer.transfer_to_station_title = item["transfer_to"]["title"];
      transfer.duration_seconds =
          static_cast<int>(item["duration"].get<double>());

      route.transfer_info = transfer;
    } else {
      Segment segment;

      segment.departure_city_title = item["from"]["title"];
      segment.departure_station_title = item["from"]["title"];
      segment.departure_time = item["departure"];

      segment.arrival_city_title = item["to"]["title"];
      segment.arrival_station_title = item["to"]["title"];
      segment.arrival_time = item["arrival"];

      segment.transport_type = item["thread"]["transport_type"];
      segment.carrier_title = item["thread"]["carrier"]["title"];

      segment.duration_seconds =
          static_cast<int>(item["duration"].get<double>());

      route.segments.push_back(segment);
    }
  }

  return route;
}

std::optional<std::vector<Route>> APIJsonExtracter::Extract(
    const json& data, const RouteRequest& request) {
  int routes_count = data["pagination"]["total"].get<int>();
  if (routes_count == 0) {
    return std::nullopt;
  }

  std::vector<Route> response;
  response.reserve(routes_count);

  for (const auto& segment : data["segments"]) {
    if (segment["has_transfers"] == false) {
      response.push_back(ParseDirectRoute(data, segment));
    } else {
      if (segment["transfers"].size() == 1) {
        response.push_back(ParseOneTransferRoute(data, segment));
      }
    }
  }

  if (response.empty()) {
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
