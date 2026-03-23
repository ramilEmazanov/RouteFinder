#include "stations_parser.hpp"

using json = nlohmann::json;

// Только вытаскивает сырой JSON
json RawStationsJsonRequester::RequestAPIJson() {
  cpr::Response stations_list = cpr::Get(
      cpr::Url{"https://api.rasp.yandex-net.ru/v3.0/stations_list/"},
      cpr::Parameters{{"apikey", "e8083d0b-419e-4c46-b6b2-e5933f3bcc4d"},
                      {"format", "json"},
                      {"lang", "ru_RU"}});

  if (stations_list.status_code != 200) {
    throw std::runtime_error("Failed to load stations list");
  }

  json stations_json = json::parse(stations_list.text);


  return stations_json;
}

// Запишет этот JSON в память
void CleanJsonWriter::WriteStationsJson(const json& data) {
  std::ofstream out("stations_map.json");
  json clean_json;
  out << clean_json.dump(2);
}

// Он уже будет создавать JSON нужного нам формата
void StationsMapMaker::BuildStationsMap() {
  json stations_json = RawStationsJsonRequester{}.RequestAPIJson();
  // Создать свой JSON формата stations_example.json из stations_json
  json clean_json;
  CleanJsonWriter{}.WriteStationsJson(clean_json);
}