#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <structs.hpp>
#include <unordered_map>
#include <vector>
#include <json_work.hpp>

using json = nlohmann::json;

struct CacheEntry {
  long long request_unix_time_;
  std::optional<std::vector<Route>> routes_;

  CacheEntry(long long request_unix_time,
             const std::optional<std::vector<Route>>& route);

  CacheEntry() = default;
};

struct Cache {
 public:
  std::optional<std::vector<Route>> Extract(const RouteRequest& request);

  void WriteToCache(const RouteRequest& request,
                    const std::optional<std::vector<Route>>& route);

  Cache(const std::string& file);

  ~Cache();

  void CheckFreshness();


  private:
  std::unordered_map<std::string, CacheEntry> cache_;
};