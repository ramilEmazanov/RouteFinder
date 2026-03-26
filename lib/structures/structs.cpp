#include "structs.hpp"

RouteRequest::RouteRequest(const std::string& from, const std::string& to,
                           const std::string& date)
    : departure_city_(from), arrival_city_(to), date_(date) {}

TransportTypes::TransportTypes() {
  map["plane"] = "Самолет";
  map["train"] = "Поезд";
  map["suburban"] = "Электричка";
  map["bus"] = "Автобус";
  map["water"] = "Морской транспорт";
  map["helicopter"] = "Вертолет";
};