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

  std::vector<std::string> Tokenize(const std::string& line);

  RouteRequest ParseTokens(const std::vector<std::string>& line);

  void PrintHelp();

};

struct RoutePrinter {
  void PrintRoutes(const std::optional<std::vector<Route>>& routes);
};
