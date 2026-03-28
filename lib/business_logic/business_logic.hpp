#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <structs.hpp>
#include <unordered_map>
#include <vector>

class IRouteRepository {
 public:
  virtual std::optional<std::vector<Route>> GetRoute(
      const RouteRequest& request) = 0;
  virtual ~IRouteRepository() = default;
};

class ICitiesDict {
 public:
  virtual std::unordered_map<std::string, std::string> MakeCitiesDict() = 0;
  virtual ~ICitiesDict() = default;
};

class MainService {
 public:
  MainService(IRouteRepository& repo, ICitiesDict& cities);

  std::optional<std::vector<Route>> ProcessRequest(const RouteRequest& request);

 private:
  bool RouteFilter(const Route& route, const RouteRequest& request);
  IRouteRepository& repo_;
  ICitiesDict& cities_;
};
