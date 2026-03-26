#pragma once

#include <iostream>
#include <string>
#include <structs.hpp>
#include <unordered_map>
#include <vector>
#include <optional>

struct IRouteRepository {
  virtual std::optional<std::vector<Route>> GetRoute(const RouteRequest& request) = 0;
  virtual ~IRouteRepository() = default;
};

struct ICitiesDict {
  virtual std::unordered_map<std::string, std::string> MakeCitiesDict() = 0;
  virtual ~ICitiesDict() = default;
};

struct MainService {
  MainService(IRouteRepository& repo, ICitiesDict& cities);

  std::optional<std::vector<Route>> ProcessRequest(const RouteRequest& request);

  bool RouteFilter(const Route& route, const RouteRequest& request);
  
 private:
  IRouteRepository& repo_;
  ICitiesDict& cities_;
  std::unordered_map<std::string, std::string> cities_dict;
};
