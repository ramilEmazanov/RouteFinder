
#pragma once

#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <unordered_map>

struct TransportTypes {
  std::unordered_map<std::string, std::string> map;
  TransportTypes();
};

struct TransferInfo {
  std::string city_title;
  std::string transfer_from_station_title;
  std::string transfer_to_station_title;
  int duration_seconds = 0;
};

struct Segment {
  std::string departure_city_title;
  std::string departure_station_title;
  std::string departure_time;

  std::string arrival_city_title;
  std::string arrival_station_title;
  std::string arrival_time;

  std::string transport_type;
  std::string carrier_title;
  int duration_seconds = 0;

};

struct Route {
  std::string departure_city_title;
  std::string arrival_city_title;

  std::string date_;

  std::vector<Segment> segments;
  std::optional<TransferInfo> transfer_info;
};

struct RouteRequest {
 public:
  RouteRequest() {}
  RouteRequest(const std::string& from, const std::string& to,
               const std::string& date);

  std::string departure_city_;
  std::string arrival_city_;

  std::string date_;

  std::optional<std::string> transport_type_;
  
  std::string has_transfers_ = "false";
};

// struct AppPaths {
//   std::filesystem::path cache_file;
//   std::filesystem::path dictionary_file;
// };