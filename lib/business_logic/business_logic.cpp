#include "business_logic.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "structs.hpp"

std::optional<std::vector<Route>> MainService::ProcessRequest(const RouteRequest& request) {
  return repo_.GetRoute(request);
}

void DataProcesser::Print() { std::cout << 1 << '\n'; }

MainService::MainService(IRouteRepository& repo) : repo_(repo) {}

MainService::MainService() {}
