#include "cache.hpp"

std::optional<std::vector<Route>> Cache::TryExtract(
    const RouteRequest& request) {
  // Тут нужно обработать:
  // 1. Не открылся файл
  // 2. Не нашелся Request
  // 3. Не открылся ДЖСОН
  std::ifstream file(file_);
  json data = json::parse(file);
  std::vector<Route> extracted_routes =
      CacheJsonExtracter{}.Extract(data, request);
  return extracted_routes;
}

void Cache::WriteToCache(const RouteRequest& request,
                         const std::optional<std::vector<Route>>& route) {
  // Открываем файл с текущим кэшом, записываем в JSON
  // Преобразовываем в JSON
  // Перезаписываем в файл
  // std::ifstream file_before(file_);
  // json data = json::parse(file_before);
  // JsonWriter{}.Write(request, route, data);
  // std::ofstream file_after(file_);
  // file_after << data.dump(2);
  std::cout << "void Cache::WriteToCache()\n";
}