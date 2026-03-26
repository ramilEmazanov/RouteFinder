#include "stations_parser.hpp"

using json = nlohmann::json;

namespace {
const long long kOneWeekInSeconds = 7 * 24 * 60 * 60;

bool IsCitiesDictFresh(long long date) {
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
        if (IsCitiesDictFresh(data["request_unix_time"].get<long long>())) {
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

std::unordered_map<std::string, std::string>
YandexCitiesDict::MakeCitiesDict() {
  std::ifstream file("cities_dict.json");

  if (IsCacheFresh(file)) {
    file.seekg(0);
    json clean_json = json::parse(file);
    return JsonToUnorderedMapMaker{}.TurnJsonToMap(clean_json);
  };

  json raw_json = RawCitiesJsonRequester{}.RequestAPIJson();
  json clean_json = CitiesDictMaker{}.BuildCitiesDict(raw_json);
  CleanJsonCitiesDictWriter{}.WriteCleanCitiesDict(clean_json);
  return JsonToUnorderedMapMaker{}.TurnJsonToMap(clean_json);
}

json RawCitiesJsonRequester::RequestAPIJson() {
  cpr::Response cities_list = cpr::Get(
      cpr::Url{"https://api.rasp.yandex-net.ru/v3.0/stations_list/"},
      cpr::Parameters{{"apikey", "e8083d0b-419e-4c46-b6b2-e5933f3bcc4d"},
                      {"format", "json"},
                      {"lang", "ru_RU"}});

  if (cities_list.status_code != 200) {
    throw std::runtime_error("Failed to load cities list");
  }

  try {
    return json::parse(cities_list.text);
  } catch (const json::parse_error& e) {
    throw std::runtime_error(std::string("Invalid JSON from API : ") +
                             e.what());
  }
}

void CleanJsonCitiesDictWriter::WriteCleanCitiesDict(const json& data) {
  std::ofstream out("cities_dict.json");
  if (!out) {
    throw std::runtime_error("Failed to open cities_dict.json for writing");
  }
  out << data.dump(2);
}

json CitiesDictMaker::BuildCitiesDict(const json& Cities_json) {
  json clean_json;

  auto epoch = std::chrono::system_clock::now().time_since_epoch();
  auto seconds =
      std::chrono::duration_cast<std::chrono::seconds>(epoch).count();

  clean_json["request_unix_time"] = seconds;
  clean_json["cities"] = json::object();

  for (const auto& country : Cities_json["countries"]) {
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

std::unordered_map<std::string, std::string>
JsonToUnorderedMapMaker::TurnJsonToMap(const json& data) {
  std::unordered_map<std::string, std::string> map;

  for (const auto& [key, value] : data["cities"].items()) {
    map[key] = value.get<std::string>();
  }

  return map;
}
