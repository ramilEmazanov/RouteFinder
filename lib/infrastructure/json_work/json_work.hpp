
#include <nlohmann/json.hpp>
#include <optional>
#include <structs.hpp>
#include <vector>

using json = nlohmann::json;

class APIJsonExtracter {
 public:
  std::optional<std::vector<Route>> Extract(const json& data,
                                            const RouteRequest& request);

 private:
  Route ParseDirectRoute(const json& data, const json& segment);

  Route ParseOneTransferRoute(const json& data, const json& segment);
};