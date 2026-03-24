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

struct RouteRespone {};

struct MainService {
  MainService();

  MainService(IRouteRepository& repo) : repo_(repo) {}

  std::optional<std::vector<Route>> ProcessRequest(const RouteRequest& request);
  // Вектор путей это уже готовые пути которые потенциально можно вывести
  // пользователю

 private:
  IRouteRepository& repo_;
};

struct DataProcesser {
  void Print();
};

struct DataPrinter {};
