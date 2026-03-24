
#pragma once

#include <string>
#include <vector>
#include <optional>

struct TransferInfo {
  std::string transfer_duration_;

  std::string transfer_point_;
};

struct Segment {
  std::string departure_title_;
  std::string departure_popular_title_;
  std::string departure_time_;

  std::string arrival_title_;
  std::string arrival_popular_title_;
  std::string arrival_time_;
  std::string transport_type_;

  std::string duration_;

  TransferInfo transfer_;
};

/*
Готовый способ добраться из одного города в другой
Он в себе хранит вектор возможных пересадок
*/
struct Route {
  std::string departure_city_;
  std::string departure_time_;

  std::string arrival_city_;
  std::string arrival_time_;

  std::vector<Segment> segments_;
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