#pragma once

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>
#include <chrono>
#include <iostream>
using json = nlohmann::json;

// Основной сервис для управления словарем городов 
struct MainStationsDictService {
  void MakeStationsDict();
};

// Только вытаскивает сырой JSON
struct RawStationsJsonRequester {
  json RequestAPIJson();
};

// Запишет этот JSON в память
struct CleanJsonStationsDictWriter {
  void WriteCleanStationsDict(const json& data);
};

// Он уже будет создавать JSON нужного нам формата
struct StationsDictMaker {
  json BuildStationsDict(const json& data);
};