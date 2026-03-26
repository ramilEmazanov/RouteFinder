#include "repos.hpp"

std::optional<std::vector<Route>> YandexRepository::GetRoute(
    const RouteRequest& request) {
  std::string url = "https://api.rasp.yandex.net/v3.0/search/";

  cpr::Response response = cpr::Get(
      cpr::Url{url},
      cpr::Parameters{{"apikey", "e8083d0b-419e-4c46-b6b2-e5933f3bcc4d"},
                      {"from", request.departure_city_},
                      {"to", request.arrival_city_},
                      {"format", "json"},
                      {"lang", "ru_RU"},
                      {"transfers", "true"},
                      {"date", request.date_}});

  if(response.status_code != 200) {
    throw std::runtime_error("Failed to make HTTP request\n");
  }

  json data = json::parse(response.text);
  std::optional<std::vector<Route>> extracted_routes =
      APIJsonExtracter{}.Extract(data, request);

  return extracted_routes;
}

std::optional<std::vector<Route>> CacheRepository::GetRoute(
    const RouteRequest& request) {
  std::optional<std::vector<Route>> extracted_routes =
      cache_.Extract(request);

  if (extracted_routes.has_value()) {
    return extracted_routes;
  }

  extracted_routes = api_repo_.GetRoute(request);
  cache_.WriteToCache(request, extracted_routes);
  return extracted_routes;
}