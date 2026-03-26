#include "structs.hpp"

RouteRequest::RouteRequest(const std::string& from, const std::string& to,
                           const std::string& date)
    : departure_city_(from), arrival_city_(to), date_(date) {}
