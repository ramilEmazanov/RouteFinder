
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Segment {
  std::string departure_city_;
  std::string arrival_city_;
  std::string transport_type_;
  std::string departure_station_;
  std::string departure_time_;
  std::string arrival_station_;
  std::string arrival_time_;
};
struct Route {
  std::string departure_city_;
  std::string arrival_city_;
  std::string transport_type_;
  std::string departure_station_;
  std::string departure_time_;
  std::string arrival_station_;
  std::string arrival_time_;
  bool has_transfers;
  std::vector<Segment> transfers_;
};

struct RouteRequest {
 public:
  RouteRequest() {}
  RouteRequest(std::vector<std::string>);

 private:
  std::string arrival_city_;
  std::string departure_city_;
  std::string departure_time_;
  std::string transport_type_;
  std::string limit_;
};