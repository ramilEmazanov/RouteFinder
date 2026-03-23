#include "json_work.hpp"

// Служебный класс для работы с Json
// Достает из переданного ему JSON кэша и Request - vector<Route>
std::vector<Route> CacheJsonExtracter::Extract(const json& data,
                                               const RouteRequest& request) {
  std::vector<Route> route;
  return route;
}

// Служебный класс для работы с JSON
// Достает из переданного ему JSON из api vector<Route>
std::vector<Route> APIJsonExtracter::Extract(const json& data,
                                             const RouteRequest& request) {
  return std::vector<Route>();
}

// Служебный класс для работы с JSON
void JsonWriter::Write(const RouteRequest& request,
                       const std::vector<Route>& route, json& data) {
  // Записывать в json в конец преобразованный route и выдавать тот же самый
  // json?
}
