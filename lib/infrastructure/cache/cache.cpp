#include "cache.hpp"

namespace {

constexpr long long kOneHourInSeconds = 60 * 60;

std::string MakeKey(const RouteRequest& request) {
  std::string response;
  response = request.departure_city_ + "|" + request.arrival_city_ + "|" +
             request.date_ + "|";

  if (request.transport_type_.has_value()) {
    response += request.transport_type_.value() + "|";
  } else {
    response += "null|";
  }

  response += request.has_transfers_;
  return response;
}
};  // namespace

CacheEntry::CacheEntry(long long request_unix_time,
                       const std::optional<std::vector<Route>>& route)
    : request_unix_time_(request_unix_time), routes_(route) {}

std::optional<std::vector<Route>> Cache::Extract(const RouteRequest& request) {
  DeleteOldRequests();
  std::optional<std::vector<Route>> response;

  if (cache_.empty()) {
    return std::nullopt;
  }

  std::string key = MakeKey(request);

  auto it = cache_.find(key);

  if (it == cache_.end()) {
    return std::nullopt;
  }

  return it->second.routes_;
}

void Cache::WriteToCache(const RouteRequest& request,
                         const std::optional<std::vector<Route>>& route) {
  DeleteOldRequests();
  std::string key = MakeKey(request);
  auto epoch = std::chrono::system_clock::now().time_since_epoch();
  long long request_unix_time =
      std::chrono::duration_cast<std::chrono::seconds>(epoch).count();

  cache_[key] = CacheEntry(request_unix_time, route);
}

void to_json(json& j, const TransferInfo& t) {
  j = json{{"city_title", t.city_title},
           {"transfer_from_station_title", t.transfer_from_station_title},
           {"transfer_to_station_title", t.transfer_to_station_title},
           {"duration_seconds", t.duration_seconds}};
}

void to_json(json& j, const Segment& s) {
  j = json{{"departure_city_title", s.departure_city_title},
           {"departure_station_title", s.departure_station_title},
           {"departure_time", s.departure_time},

           {"arrival_city_title", s.arrival_city_title},
           {"arrival_station_title", s.arrival_station_title},
           {"arrival_time", s.arrival_time},

           {"transport_type", s.transport_type},
           {"carrier_title", s.carrier_title},
           {"duration_seconds", s.duration_seconds}};
}

void to_json(json& j, const Route& r) {
  j = json{{"departure_city_title", r.departure_city_title},
           {"arrival_city_title", r.arrival_city_title},
           {"date", r.date_},
           {"segments", r.segments},
           {"transfer_info", r.transfer_info}};
}

void to_json(json& j, const CacheEntry& entry) {
  j = json{{"request_unix_time", entry.request_unix_time_},
           {"routes", entry.routes_}};
}

Cache::~Cache() {
  json j = cache_;
  std::ofstream out("cache.json");

  if (!out.is_open()) {
    return;
  }

  out << j.dump(2);
}

void from_json(const json& j, TransferInfo& t) {
  j.at("city_title").get_to(t.city_title);
  j.at("transfer_from_station_title").get_to(t.transfer_from_station_title);
  j.at("transfer_to_station_title").get_to(t.transfer_to_station_title);
  j.at("duration_seconds").get_to(t.duration_seconds);
}

void from_json(const json& j, Segment& s) {
  j.at("departure_city_title").get_to(s.departure_city_title);
  j.at("departure_station_title").get_to(s.departure_station_title);
  j.at("departure_time").get_to(s.departure_time);

  j.at("arrival_city_title").get_to(s.arrival_city_title);
  j.at("arrival_station_title").get_to(s.arrival_station_title);
  j.at("arrival_time").get_to(s.arrival_time);

  j.at("transport_type").get_to(s.transport_type);
  j.at("carrier_title").get_to(s.carrier_title);
  j.at("duration_seconds").get_to(s.duration_seconds);
}

void from_json(const json& j, Route& r) {
  j.at("departure_city_title").get_to(r.departure_city_title);
  j.at("arrival_city_title").get_to(r.arrival_city_title);

  j.at("date").get_to(r.date_);

  j.at("segments").get_to(r.segments);

  if (j.contains("transfer_info") && !j.at("transfer_info").is_null()) {
    r.transfer_info = j.at("transfer_info").get<TransferInfo>();
  } else {
    r.transfer_info = std::nullopt;
  }
}

void from_json(const json& j, CacheEntry& entry) {
  j.at("request_unix_time").get_to(entry.request_unix_time_);

  if (j.contains("routes") && !j.at("routes").is_null()) {
    entry.routes_ = j.at("routes").get<std::vector<Route>>();
  } else {
    entry.routes_ = std::nullopt;
  }
}

void from_json(const json& j,
               std::unordered_map<std::string, CacheEntry>& cache) {
  cache.clear();

  for (const auto& [key, value] : j.items()) {
    cache[key] = value.get<CacheEntry>();
  }
}

Cache::Cache(const std::string& file) {
  std::ifstream in(file);

  if (!in.is_open()) {
    return;
  }

  
  try {
    json data = json::parse(in);
    cache_ = data.get<std::unordered_map<std::string, CacheEntry>>();
    DeleteOldRequests();
  } catch (std::exception& e) {
    cache_.clear();
  }
}

void Cache::DeleteOldRequests() {
  auto epoch = std::chrono::system_clock::now().time_since_epoch();
  long long now =
      std::chrono::duration_cast<std::chrono::seconds>(epoch).count();

  for (auto it = cache_.begin(); it != cache_.end();) {
    long long request_time = it->second.request_unix_time_;

    if (now - request_time > kOneHourInSeconds) {
      cache_.erase(it);
    } else {
      it++;
    }
  }
}
