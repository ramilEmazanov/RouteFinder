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
    std::cout << "\n1. Новый запрос\n2. Помощь\n3. Выход\n> ";

    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "3") {
      break;
    }

    if (cmd == "2") {
      parser.PrintHelp();
      continue;
    }

    if (cmd != "1") {
      std::cout << "Неизвестная команда.\n";
      continue;
    }

    try {
      RouteRequest request = parser.ReadInteractiveRequest();
      std::optional<std::vector<Route>> routes =
          service.ProcessRequest(request);

      printer.PrintRoutes(routes);
    } catch (const std::exception& e) {
      std::cout << "Error: " << e.what() << '\n';
    }
  }

  return 0;
}