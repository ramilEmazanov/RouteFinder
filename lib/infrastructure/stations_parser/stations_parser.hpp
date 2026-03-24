#pragma once

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>
#include <chrono>
#include <iostream>
#include <business_logic.hpp>
using json = nlohmann::json;

// Основной сервис для управления словарем городов 
struct YandexCitiesDict : ICitiesDict {
  std::unordered_map<std::string, std::string> MakeCitiesDict();
};

// Только вытаскивает сырой JSON
struct RawCitiesJsonRequester {
  json RequestAPIJson();
};

// Запишет этот JSON в память
struct CleanJsonCitiesDictWriter {
  void WriteCleanCitiesDict(const json& data);
};

// Он уже будет создавать JSON нужного нам формата
struct CitiesDictMaker {
  json BuildCitiesDict(const json& data);
};

struct JsonToUnorderedMapMaker {
  std::unordered_map<std::string, std::string> TurnJsonToMap(const json& data);
};