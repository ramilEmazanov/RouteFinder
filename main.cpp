#include <cpr/cpr.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "business_logic.hpp"
#include "infrastructure.hpp"

using json = nlohmann::json;

int main() {
  
  
  
  // RouteRequest request(Request);

  // std::vector<std::string> Request = {"From", "To", "Date", "Transport"};


  // DataRequester requester(request);

  // Cache cache(requester);

  // MainService service(request);

  // service.PrintAnswer();

  // cpr::Response r =
  //     cpr::Get(cpr::Url{"https://api.rasp.yandex-net.ru/v3.0/search/"},
  //              cpr::Parameters{{"apikey",
  //              "e8083d0b-419e-4c46-b6b2-e5933f3bcc4d"},
  //                              {"from", "c146"},
  //                              {"to", "c213"},
  //                              {"format", "json"},
  //                              {"lang", "ru_RU"},
  //                              {"date", "2026-03-19"}});

  // std::cout << r.text;

  // // std::ifstream f("test.json");
  // json data = json::parse(r.text);
  // std::ofstream out("request2.json");
  // out << data.dump(2);
  // // float n = data["pi"].get();
  // std::cout << data.at(0);
  // return 0;
}
