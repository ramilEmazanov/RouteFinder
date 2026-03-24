#pragma once

#include <optional>
#include <stdexcept>
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
  void PrintRoute(std::string routes);
};