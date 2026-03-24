#include "presentation.hpp"

void CLIParser::EnsureValueExists(int i, int argc, const std::string& arg) {
  if (i + 1 >= argc) {
    throw std::runtime_error("Missing value for " + arg);
  }
  std::vector<std::string> flags = {"--from", "--to", "--date", "--transport"};

  for (const auto& flag : flags) {
    if (flag == arg) {
      throw std::runtime_error("Missing value for " + arg);
    }
  }
}

RouteRequest CLIParser::ParseCLI(int argc, const char** argv) {
  RouteRequest request{};

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "--from") {
      EnsureValueExists(i, argc, argv[i + 1]);
      request.departure_city_ = argv[++i];
    }
    if (arg == "--to") {
      EnsureValueExists(i, argc, argv[i + 1]);
      request.arrival_city_ = argv[++i];
    }
    if (arg == "--date") {
      EnsureValueExists(i, argc, argv[i + 1]);
      request.date_ = argv[++i];
    }
    if (arg == "--transport") {
      EnsureValueExists(i, argc, argv[i + 1]);
      request.transport_type_ = argv[++i];
    }
    if (arg == "--transfers") {
      request.has_transfers_ = "true";
    }
  }
  if (request.departure_city_.empty() || request.arrival_city_.empty() ||
      request.date_.empty()) {
    throw std::runtime_error("No required argument");
  }

  return request;
}

void RoutePrinter::PrintRoute(std::string route) {}