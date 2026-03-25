
#include <nlohmann/json.hpp>
#include <structs.hpp>
#include <vector>
#include <optional>

using json = nlohmann::json;

// Служебный класс для работы с Json
// Достает из переданного ему JSON кэша и Request - vector<Route>
struct CacheJsonExtracter {
  std::vector<Route> Extract(const json& data, const RouteRequest& request);
};

struct APIJsonExtracter {
  std::optional<std::vector<Route>> Extract(const json& data, const RouteRequest& request);

  Route ParseDirectRoute(const json& data, const json& segment);

  Route ParseOneTransferRoute(const json& data, const json& segment);

};

// Служебный класс для работы с JSON
struct JsonWriter {
  void Write(const RouteRequest& request, const std::vector<Route>& route,
             json& data);
};
