#include <cpr/cpr.h>

#include <business_logic.hpp>
#include <cache.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <presentation.hpp>
#include <repos.hpp>
#include <stations_parser.hpp>
#include <vector>

using json = nlohmann::json;

int main(int argc, const char** argv) {
  try {
    RouteRequest route = CLIParser{}.ParseCLI(argc, argv);

    YandexRepository yandex_repo{};
    
    Cache cache("cache.json");
    CacheRepository cache_repo(yandex_repo, cache);
    
    YandexCitiesDict yandex_cities{};

    MainService service{cache_repo, yandex_cities};

    std::optional<std::vector<Route>> routes = service.ProcessRequest(route);
    RoutePrinter{}.PrintRoutes(routes);

  } catch (const std::exception& e) {
    std::cout << e.what();
  }
  return 0;
}