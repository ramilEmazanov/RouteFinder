#pragma once

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Только вытаскивает сырой JSON
struct RawStationsJsonRequester {
  json RequestAPIJson();
};

// Запишет этот JSON в память
struct CleanJsonWriter {
  void WriteStationsJson(const json& data);
};

// Он уже будет создавать JSON нужного нам формата
struct StationsMapMaker {
  void BuildStationsMap();
};