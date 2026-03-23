#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

#include <business_logic.hpp>
#include <structs.hpp>
#include <cache.hpp>

// YandexRepository внутри себя:
// делает HTTP-запрос
// получает JSON
// парсит JSON
// превращает в vector<Route>
struct YandexRepository : IRouteRepository {
 public:
  std::vector<Route> GetRoute(const RouteRequest& request) override;
  void Print();
 private:
};

// pattern caching proxy
struct CacheRepository : IRouteRepository {
 public:
  CacheRepository(IRouteRepository& yandex, Cache& cache)
      : api_repo_(yandex), cache_(cache) {}

  std::vector<Route> GetRoute(const RouteRequest& request) override;

 private:
  Cache& cache_;
  IRouteRepository& api_repo_;
};