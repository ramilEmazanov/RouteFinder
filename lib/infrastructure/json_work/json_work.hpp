
#include <nlohmann/json.hpp>
#include <structs.hpp>
#include <vector>
#include <optional>

using json = nlohmann::json;

struct APIJsonExtracter {
  std::optional<std::vector<Route>> Extract(const json& data, const RouteRequest& request);

  Route ParseDirectRoute(const json& data, const json& segment);

  Route ParseOneTransferRoute(const json& data, const json& segment);

};