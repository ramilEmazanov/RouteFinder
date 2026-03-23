#include "stations_parser.hpp"

using json = nlohmann::json;

namespace {
const long long kOneWeekInSeconds = 7 * 24 * 60 * 60;

bool IsStationsDictFresh(long long date) {
  auto epoch = std::chrono::system_clock::now().time_since_epoch();
  long long now =
      std::chrono::duration_cast<std::chrono::seconds>(epoch).count();

  if (date > now) return false;

  return now - date <= kOneWeekInSeconds;
}

bool IsCacheFresh(std::ifstream& file) {
  bool is_fresh = false;

  if (file.is_open()) {
    json data;
    try {
      data = json::parse(file);

      if (data.contains("request_unix_time") &&
          data["request_unix_time"].is_number()) {
        if (IsStationsDictFresh(data["request_unix_time"].get<long long>())) {
          is_fresh = true;
        }
      }

    } catch (const json::parse_error& e) {
      is_fresh = false;
      std::cerr << "Parsing JSON error : " << " " << e.what() << '\n';
    }
  }

  return is_fresh;
}
};  // namespace

void MainStationsDictService::MakeStationsDict() {
  std::ifstream file("stations_dictionary.json");

  if(IsCacheFresh(file)) return;

  json raw_json = RawStationsJsonRequester{}.RequestAPIJson();
  json clean_json = StationsDictMaker{}.BuildStationsDict(raw_json);
  CleanJsonStationsDictWriter{}.WriteCleanStationsDict(clean_json);
}

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

  try {
    return json::parse(stations_list.text);
  } catch (const json::parse_error& e) {
    throw std::runtime_error(std::string("Invalid JSON from API : ") +
                             e.what());
  }
}

// Запишет этот JSON в память
void CleanJsonStationsDictWriter::WriteCleanStationsDict(const json& data) {
  std::ofstream out("stations_dictionary.json");
  if (!out) {
    throw std::runtime_error("Failed to open stations_Dict.json for writing");
  }
  out << data.dump(2);
}

// Он уже будет создавать JSON нужного нам формата
json StationsDictMaker::BuildStationsDict(const json& stations_json) {
  json clean_json;

  auto epoch = std::chrono::system_clock::now().time_since_epoch();
  auto seconds =
      std::chrono::duration_cast<std::chrono::seconds>(epoch).count();

  clean_json["request_unix_time"] = seconds;
  clean_json["cities"] = json::object();

  for (const auto& country : stations_json["countries"]) {
    for (const auto& region : country["regions"]) {
      for (const auto& settlement : region["settlements"]) {
        if (settlement["codes"].contains("yandex_code") &&
            settlement["codes"]["yandex_code"].is_string()) {
          std::string title = settlement["title"].get<std::string>();
          std::string code =
              settlement["codes"]["yandex_code"].get<std::string>();

          clean_json["cities"][title] = code;
        }
      }
    }
  }
  return clean_json;
}