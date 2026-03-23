#pragma once
#include <cpr/cpr.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <structs.hpp>
#include <json_work.hpp>

using json = nlohmann::json;

struct Cache {
 public:
  std::optional<std::vector<Route>> TryExtract(const RouteRequest& request);

  void WriteToCache(const RouteRequest& request,
                    const std::vector<Route>& route);

  Cache() = default;
  Cache(std::string cache_file) : file_(cache_file) {}

 private:
  std::string file_;
};