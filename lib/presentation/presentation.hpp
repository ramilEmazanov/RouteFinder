#pragma once

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <structs.hpp>
#include <unordered_set>
#include <vector>

struct CLIParser {
  RouteRequest ReadInteractiveRequest();
  void PrintHelp();

 private:
  std::string AskNonEmptyString(const std::string& request);
  std::string AskDate();
  std::optional<std::string> AskTransport();
  std::string AskTransfers();
};

struct RoutePrinter {
  void PrintRoutes(const std::optional<std::vector<Route>>& routes);

  void PrintTransferInfo(const TransferInfo& transfer);

  void PrintSegment(const Segment& segment, int index);
};
