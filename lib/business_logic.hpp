#pragma once

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

struct Route {};

struct RouteRequest {
 public:
  RouteRequest() {}
  RouteRequest(std::vector<std::string>);

 private:
  std::string from_;
  std::string to_;
  std::string date_;
  std::string transport_type_;
  std::string limit_;
};

struct IRouteRepository {
  virtual std::vector<Route> GetRoute(const RouteRequest& request) = 0;
  virtual ~IRouteRepository() = default;
};

struct RouteRespone {};

struct DataRequester {
  DataRequester() {}

  std::vector<Route> ProcessRequest(RouteRequest request) {
    repo_->GetRoute(request);
    std::vector<Route> temp;
    return temp;
  }

 private:
  IRouteRepository* repo_;
};

struct DataProcesser {};

struct DataPrinter {};
