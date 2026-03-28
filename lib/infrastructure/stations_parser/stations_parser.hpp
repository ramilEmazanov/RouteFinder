#pragma once

#include <cpr/cpr.h>

#include <business_logic.hpp>
#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class YandexCitiesDict : public ICitiesDict {
 public:
  std::unordered_map<std::string, std::string> MakeCitiesDict();
};

class RawCitiesJsonRequester {
 public:
  json RequestAPIJson();
};

class CleanJsonCitiesDictWriter {
 public:
  void WriteCleanCitiesDict(const json& data);
};

class CitiesDictMaker {
 public:
  json BuildCitiesDict(const json& data);
};

class JsonToUnorderedMapMaker {
 public:
  std::unordered_map<std::string, std::string> TurnJsonToMap(const json& data);
};