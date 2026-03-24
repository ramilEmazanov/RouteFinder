#pragma once

#include <optional>
#include <stdexcept>
#include <iostream>
#include <string>
#include <structs.hpp>
#include <vector>

struct CLIParser {
 public:
  RouteRequest ParseCLI(int argc, const char** argv);

 private:
  void EnsureValueExists(int i, int argc, const std::string& arg);
};

struct RoutePrinter {
  void PrintRoutes(const std::optional<std::vector<Route>>& routes);
};
