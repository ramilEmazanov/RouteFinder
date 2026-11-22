#include "structs.hpp"
#include <string>

long long Route::DurationSeconds() const {
  long long duration = transfer_info ? transfer_info->duration_seconds : 0;
  for (const auto& segment : segments) {
    duration += segment.duration_seconds;
  }
  return duration;
}

RouteRequest::RouteRequest(const std::string& from, const std::string& to,
                           const std::string& date)
    : departure_city_(from), arrival_city_(to), date_(date) {}
