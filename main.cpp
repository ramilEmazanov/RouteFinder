#include <cpr/cpr.h>

#include <business_logic.hpp>
#include <cache.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
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

  cpr::Response r =
      cpr::Get(cpr::Url{"https://api.rasp.yandex-net.ru/v3.0/search/"},
               cpr::Parameters{{"apikey",
               "e8083d0b-419e-4c46-b6b2-e5933f3bcc4d"},
                               {"from", "c5"},
                               {"to", "c213"},
                               {"format", "json"},
                               {"lang", "ru_RU"},
                               {"date", "2026-03-24"}});

  // std::cout << r.text;

  json data = json::parse(r.text);
  std::ofstream out("ivanovo_msk.json", std::ios::trunc);
  out << data.dump(2);
  return 0;
}

// c5 ivanovo