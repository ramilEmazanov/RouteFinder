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
  // try {
  //   MainStationsDictService{}.MakeStationsDict();
  // } catch (std::exception& e) {
  //   std::cout << e.what();
  //   return 1;
  // }

  // StationsMapMaker{}.BuildStationsMap();
  // std::ifstream cities_map("stations_map.json");
  // json map = json::parse(cities_map);
  // std::cout << map["request_date"] << '\n';
  // std::cout << map["cities"]["Уфа"] << '\n';

  // RouteRequest request = CLIParser{}.ParseCLI(argc, argv);
  // std::string file_path = "Cache.json";
  // Cache cache(file_path);

  // YandexRepository yandex_api;
  // CacheRepository cache_repo(yandex_api, cache);

  // MainService requester(CacheRepository);

  // std::vector<std::string> Request = {"From", "To", "Date", "Transport"};

  // DataRequester requester(request);

  // Cache cache(requester);

  // MainService service(request);

  // service.PrintAnswer();

  // cpr::Response r =
  //     cpr::Get(cpr::Url{"https://api.rasp.yandex-net.ru/v3.0/search/"},
  //              cpr::Parameters{{"apikey",
  //              "e8083d0b-419e-4c46-b6b2-e5933f3bcc4d"},
  //                              {"from", "c5"},
  //                              {"to", "c172"},
  //                              {"format", "json"},
  //                              {"lang", "ru_RU"},
  //                              {"transfers", "true"},
  //                              {"date", "2026-05-24"}});

  // std::cout << r.text;

  // std::ifstream in("kazan_ufa.json");
  // json data = json::parse(in);

  try {
    RouteRequest route = CLIParser{}.ParseCLI(argc, argv);

    MainStationsDictService{}.MakeStationsDict();

    YandexRepository yandex_repo{};

    std::optional<std::vector<Route>> response =
        MainService{yandex_repo}.ProcessRequest(route);

    RoutePrinter{}.PrintRoute("lol");
    // std::optional<std::vector<Route>> routes =
    //     YandexRepository{}.GetRoute(route);

    // for (const auto& route : *routes) {
    //   std::cout << route.departure_city_ << '\n';
    //   std::cout << route.arrival_city_ << '\n';
    //   std::cout << route.departure_time_ << '\n';
    //   std::cout << route.arrival_time_ << '\n';
    //   std::cout << '\n';
    // }
  } catch (const std::exception& e) {
    std::cout << e.what();
  }
  // std::ofstream out("kazan_ufa_2.json", std::ios::trunc);
  // out << data.dump(2);
  return 0;
}

// c5 ivanovo
// c172 ivanovo