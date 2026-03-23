
#include <nlohmann/json.hpp>
#include <structs.hpp>
#include <vector>

using json = nlohmann::json;

// Служебный класс для работы с Json
// Достает из переданного ему JSON кэша и Request - vector<Route>
struct CacheJsonExtracter {
  std::vector<Route> Extract(const json& data, const RouteRequest& request);
};

// Служебный класс для работы с JSON
// Достает из переданного ему JSON из api vector<Route>
struct APIJsonExtracter {
  std::vector<Route> Extract(const json& data, const RouteRequest& request);
};

// Служебный класс для работы с JSON
struct JsonWriter {
  void Write(const RouteRequest& request, const std::vector<Route>& route,
             json& data);
};
