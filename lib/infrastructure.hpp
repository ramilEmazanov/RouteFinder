// infrastructure.hpp
#pragma once
#include <cpr/cpr.h>

#include <fstream>
#include <nlohmann/json.hpp>

#include "business_logic.hpp"

using json = nlohmann::json;

// Служебный класс для работы с Json
// Достает из переданного ему JSON и Request - vector<Route>
struct CacheJsonExtracter {
  std::vector<Route> Extract(const json& data, const RouteRequest& request) {
    std::vector<Route> route;
    return route;
  }
};

struct APIJsonExtracter {
  std::vector<Route> Extract(const json& data, const RouteRequest& request) {
    std::vector<Route> route;
    return route;
  }
};

// Служебный класс для работы с JSON
struct JsonWriter {
  void Write(const RouteRequest& request, const std::vector<Route>& route,
             json& data) {
    // Записывать в json в конец преобразованный route и выдавать тот же самый
    // json?
  }
};

struct Cache {
 public:
  std::optional<std::vector<Route>> TryExtract(const RouteRequest& request) {
    std::ifstream file(file_);
    json data = json::parse(file);
    std::vector<Route> extracted_routes =
        CacheJsonExtracter{}.Extract(data, request);
    return extracted_routes;
  }

  void WriteToCache(const RouteRequest& request,
                    const std::vector<Route>& route) {
    // Открываем файл с текущим кэшом, записываем в JSON
    // Преобразовываем в JSON
    // Перезаписываем в файл
    std::ifstream file_before(file_);
    json data = json::parse(file_before);
    JsonWriter{}.Write(request, route, data);
    std::ofstream file_after(file_);
    file_after << data.dump(2);
  }

  // std::vector<Route> ExtractFromCache(RouteRequest& request) {
  //   std::ifstream file(file_);
  //   json data = json::parse(file);
  //   std::vector<Route> extracted_routes =
  //       CacheJsonExtracter{}.Extract(data, request);
  //   return extracted_routes;
  // }

  Cache() = delete;
  Cache(std::string cache_file) : file_(cache_file) {}

 private:
  std::string file_;
};

// YandexRepository внутри себя:
// делает HTTP-запрос
// получает JSON
// парсит JSON
// превращает в vector<Route>
struct YandexRepository : IRouteRepository {
 public:
  std::vector<Route> GetRoute(const RouteRequest& request) {
    std::string url = "https://api.rasp.yandex.net/v3.0/search/";
    cpr::Response response = cpr::Get(cpr::Url{url});
    json data = json::parse(response.text);
    std::vector<Route> extracted_routes =
        APIJsonExtracter{}.Extract(data, request);
    return extracted_routes;
  }

 private:
  // Как не хранить парсер как член класса?
  // RawJsonProcesser json_parser;
};

struct CacheRepository : IRouteRepository {
 public:
  CacheRepository(IRouteRepository& yandex, Cache& cache)
      : api_repo_(yandex), cache_(cache) {}

  std::vector<Route> GetRoute(const RouteRequest& request) override {
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

 private:
  Cache& cache_;
  // RawJsonProcesser json_parser;
  IRouteRepository& api_repo_;
};