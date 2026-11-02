#pragma once

#include <cpr/cpr.h>

#include <business_logic.hpp>
#include <cache.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <structs.hpp>

class YandexRepository : public IRouteRepository {
 public:
  YandexRepository(const std::string& api_key);
  std::optional<std::vector<Route>> GetRoute(
      const RouteRequest& request) override;

 private:
  std::string api_key_;
};

class CacheRepository : public IRouteRepository {
 public:
  CacheRepository(IRouteRepository& yandex, Cache& cache)
      : api_repo_(yandex), cache_(cache) {}

  std::optional<std::vector<Route>> GetRoute(
      const RouteRequest& request) override;

 private:
  Cache& cache_;
  IRouteRepository& api_repo_;
};