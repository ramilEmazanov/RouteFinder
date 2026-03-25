#include "cache.hpp"

namespace {
std::string MakeKey(const RouteRequest& request) {
  std::string response;
  response = request.departure_city_ + "|" + request.arrival_city_ + "|" +
             request.date_ + "|";
  if (request.transport_type_.has_value()) {
    response += request.transport_type_.value();
  } else {
    response += "null";
  }

  response += request.has_transfers_;
  return response;
}
};  // namespace

CacheEntry::CacheEntry(long long request_unix_time,
                       const std::optional<std::vector<Route>>& route)
    : request_unix_time_(request_unix_time), routes_(route) {}

std::optional<std::vector<Route>> Cache::Extract(const RouteRequest& request) {
  std::optional<std::vector<Route>> response;

  if (!cache_.has_value()) {
    return std::nullopt;
  }

  std::string key = MakeKey(request);

  auto it = cache_.value().find(key);

  if (it == cache_.value().end()) {
    return std::nullopt;
  }

  const CacheEntry& entry = it->second;
  return entry.routes_;
}

void Cache::WriteToCache(const RouteRequest& request,
                         const std::optional<std::vector<Route>>& route) {
  std::string key = MakeKey(request);
  auto now = std::chrono::system_clock::now();
  long long request_unix_time = now.time_since_epoch().count();

  CacheEntry entry(request_unix_time, route);

  cache_.value().emplace(key, entry);
}

Cache::Cache(const std::string& file)
    : cache_(std::unordered_map<std::string, CacheEntry>{}) {
  std::ifstream in(file);

  if (in.is_open()) {
    json data = json::parse(in);
    // Записать из файла данные в мапу
    std::pair<std::string, std::vector<Route>> key_value =
        CacheMaker{}.Extract(data);
  }
}

Cache::~Cache() {
  // Выгрузить данные из кэша
  if (cache_.has_value()) {
    for (const auto& [key, value] : cache_.value()) {
      CacheWriter{}.Write(key, value);
    }
  }
}