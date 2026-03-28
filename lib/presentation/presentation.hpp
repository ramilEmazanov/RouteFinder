#pragma once

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <structs.hpp>
#include <unordered_set>
#include <vector>

class CLIParser {
 public:
  RouteRequest ReadInteractiveRequest();
  void PrintHelp();

 private:
  std::string AskNonEmptyString(const std::string& request);
  std::string AskDate();
  std::optional<std::string> AskTransport();
  std::string AskTransfers();
};

class RoutePrinter {
 public:
  void PrintRoutes(const std::optional<std::vector<Route>>& routes) const;

 private:
  void PrintTransferInfo(const TransferInfo& transfer) const;

  void PrintSegment(const Segment& segment, int index) const;
};
