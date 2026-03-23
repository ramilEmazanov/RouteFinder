#pragma once

#include <string>
#include <vector>

#include <structs.hpp>

struct CLIParser {
  RouteRequest ParseCLI(int argc, const char** argv) {
    return RouteRequest{};
  }
};

struct RoutePrinter {
  void PrintRoute(std::string routes) {}
};