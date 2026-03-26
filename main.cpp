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
  CLIParser parser;
  YandexRepository yandex_repo{};
  Cache cache("cache.json");
  
  CacheRepository cache_repo(yandex_repo, cache);
  YandexCitiesDict yandex_cities{};
  MainService service{cache_repo, yandex_cities};
  RoutePrinter printer;

  while (true) {
    std::cout << "> ";

    std::string line;
    if (!std::getline(std::cin, line)) {
      break;
    }

    if (line.empty()) {
      continue;
    }

    if (line == "quit") {
      break;
    }

    if (line == "help") {
      parser.PrintHelp();
      continue;
    }

    try {
      std::vector<std::string> tokens = parser.Tokenize(line);
      RouteRequest request = parser.ParseTokens(tokens);
      std::optional<std::vector<Route>> routes =
          service.ProcessRequest(request);

      printer.PrintRoutes(routes);
    } catch (const std::exception& e) {
      std::cout << "Error: " << e.what() << '\n';
    }
  }

  return 0;
}