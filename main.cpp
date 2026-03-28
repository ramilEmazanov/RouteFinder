#include <business_logic.hpp>
#include <cache.hpp>
#include <iostream>
#include <presentation.hpp>
#include <repos.hpp>
#include <string>
#include <optional>
#include <exception>
#include <stations_parser.hpp>

int main(int argc, const char** argv) {
  if(argc == 1) {
    std::cerr << "No API key provided\n";
    return 1;
  }
  std::string api_key = argv[1];
  CLIParser parser;
  YandexRepository yandex_repo{api_key};
  Cache cache(PathHelper{}.CachePath());

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