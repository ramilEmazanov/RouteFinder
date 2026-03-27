#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <json_work.hpp>
#include <nlohmann/json.hpp>
#include <structs.hpp>
#include <unordered_map>
#include <vector>

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
  static constexpr int kDefaultCacheSize = 10;

  std::optional<std::vector<Route>> Extract(const RouteRequest& request);

  void WriteToCache(const RouteRequest& request,
                    const std::optional<std::vector<Route>>& route);

  Cache(const std::string& file, int cache_size = kDefaultCacheSize);

  ~Cache();

  void DeleteOldRequests();

  void ControlMaxCacheSize();

 private:
  int cache_max_size;
  std::string file_;
  std::unordered_map<std::string, CacheEntry> cache_;
};

void to_json(json& j, const TransferInfo& t);

void to_json(json& j, const Segment& s);

void to_json(json& j, const Route& r);

void to_json(json& j, const CacheEntry& entry);

void from_json(const json& j, TransferInfo& t);

void from_json(const json& j, CacheEntry& entry);

void from_json(const json& j,
               std::unordered_map<std::string, CacheEntry>& cache);