#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <vector>

#include <business_logic.hpp>
#include <cache.hpp>
#include <repos.hpp>
// #include "presentation.hpp"

using json = nlohmann::json;

int main(int argc, const char** argv) {
  
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
  //              cpr::Parameters{{"apikey", "e8083d0b-419e-4c46-b6b2-e5933f3bcc4d"},
  //                              {"from", "c212"},
  //                              {"to", "c213"},
  //                              {"format", "json"},
  //                              {"lang", "ru_RU"},
  //                              {"date", "2026-03-21"}});


  // cpr::Response stations_list =
  //     cpr::Get(cpr::Url{"https://api.rasp.yandex-net.ru/v3.0/stations_list/"},
  //              cpr::Parameters{{"apikey", "e8083d0b-419e-4c46-b6b2-e5933f3bcc4d"},
  //                              {"format", "json"},
  //                              {"lang", "ru_RU"}});

  // // std::cout << r.text;
  
  // // std::ifstream f("request.json");
  // json data = json::parse(stations_list.text);
  // std::ofstream out("stations1.json", std::ios::trunc);
  // out << data.dump(2);
  // return 0;
}
