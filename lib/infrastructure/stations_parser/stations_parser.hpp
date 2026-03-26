#pragma once

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>
#include <chrono>
#include <iostream>
#include <business_logic.hpp>
using json = nlohmann::json;

struct YandexCitiesDict : ICitiesDict {
  std::unordered_map<std::string, std::string> MakeCitiesDict();
};

struct RawCitiesJsonRequester {
  json RequestAPIJson();
};

struct CleanJsonCitiesDictWriter {
  void WriteCleanCitiesDict(const json& data);
};

struct CitiesDictMaker {
  json BuildCitiesDict(const json& data);
};

struct JsonToUnorderedMapMaker {
  std::unordered_map<std::string, std::string> TurnJsonToMap(const json& data);
};