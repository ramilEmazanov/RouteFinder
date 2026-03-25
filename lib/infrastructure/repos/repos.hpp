#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

#include <business_logic.hpp>
#include <structs.hpp>
#include <cache.hpp>

struct YandexRepository : IRouteRepository {
 public:
  std::optional<std::vector<Route>> GetRoute(const RouteRequest& request) override;
 private:
};

struct CacheRepository : IRouteRepository {
 public:
  CacheRepository(IRouteRepository& yandex, Cache& cache)
      : api_repo_(yandex), cache_(cache) {}

  std::optional<std::vector<Route>> GetRoute(const RouteRequest& request) override;

 private:
  Cache& cache_;
  IRouteRepository& api_repo_;
};