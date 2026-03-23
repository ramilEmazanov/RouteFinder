#include "repos.hpp"
// YandexRepository внутри себя:
// делает HTTP-запрос
// получает JSON
// парсит JSON
// превращает в vector<Route>
std::vector<Route> YandexRepository::GetRoute(const RouteRequest& request) {
  std::string url = "https://api.rasp.yandex.net/v3.0/search/";
  cpr::Response response = cpr::Get(cpr::Url{url});
  json data = json::parse(response.text);
  std::vector<Route> extracted_routes =
      APIJsonExtracter{}.Extract(data, request);

  return extracted_routes;
}

void YandexRepository::Print() {
  std::cout << "YandexRepository::Print()\n";
}

// pattern caching proxy

std::vector<Route> CacheRepository::GetRoute(
    const RouteRequest& request) {
  auto cached_value = cache_.TryExtract(request);
  std::vector<Route> extracted_routes;

  if (cached_value.has_value()) {
    extracted_routes = cached_value.value();
    return extracted_routes;
  }

  extracted_routes = api_repo_.GetRoute(request);
  cache_.WriteToCache(request, extracted_routes);
  return extracted_routes;
}