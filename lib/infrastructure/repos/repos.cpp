#include "repos.hpp"

#include <cpr/cpr.h>

#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

using json = nlohmann::json;

YandexRepository::YandexRepository(const std::string& api_key)
    : api_key_(api_key) {}

std::optional<std::vector<Route>> YandexRepository::GetRoute(
    const RouteRequest& request) {
  try {
    std::string url = "https://api.rasp.yandex.net/v3.0/search/";

    cpr::Response response = cpr::Get(
        cpr::Url{url}, cpr::Parameters{{"apikey", api_key_},
                                       {"from", request.departure_city_},
                                       {"to", request.arrival_city_},
                                       {"format", "json"},
                                       {"lang", "ru_RU"},
                                       {"transfers", request.has_transfers_},
                                       {"date", request.date_}});

    if (response.error.code != cpr::ErrorCode::OK) {
      throw std::runtime_error("HTTP request failed : " +
                               response.error.message);
    }

    if (response.status_code != 200) {
      throw std::runtime_error("Yandex API returned error : " +
                               std::to_string(response.status_code));
    }

    json data = json::parse(response.text);
    std::optional<std::vector<Route>> extracted_routes =
        APIJsonExtracter{}.Extract(data, request);

    return extracted_routes;
  } catch (const std::exception& e) {
    throw e;
  }
}

std::optional<std::vector<Route>> CacheRepository::GetRoute(
    const RouteRequest& request) {
  std::optional<std::vector<Route>> extracted_routes;

  try {
    extracted_routes = cache_.Extract(request);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  if (extracted_routes.has_value()) {
    return extracted_routes;
  }

  try {
    extracted_routes = api_repo_.GetRoute(request);
    cache_.WriteToCache(request, extracted_routes);
    return extracted_routes;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return std::nullopt;
  }
}