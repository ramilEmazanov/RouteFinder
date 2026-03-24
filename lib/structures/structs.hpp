
#pragma once

#include <string>
#include <vector>
#include <optional>

struct TransferInfo {
  std::string transfer_duration_;

  std::string transfer_point_;
};

struct Segment {
  std::string departure_title_;
  std::string departure_time_;

  std::string arrival_title_;
  std::string arrival_time_;

  std::string transport_type_;

  std::string duration_;

  TransferInfo transfer_;
};

/*
Без пересадок:

Город отправления: Уфа 
Город прибытия: Казань

Путь №1:
Станция отправления: Уфа, Южный Автовокзал
Станция прибытия: Казань, Восточный автовокзал
Время отправления: 2026-05-25 13:00
Время прибытия: 2026-05-25 16:00
Длительность: 28800 секунд
Стоимость билета: 1600 RUB
Тип транспорта: Автобус

Путь №2:
.....
*/

/*
Захардкодить скип если transfers.size() > 1
С пересадками:

Город отправления: Уфа 
Город прибытия: Баку

Путь №1:
Количество пересадок: 1
Город пересадки: Москва

Отправление: Уфа, аэропорт
Город пересадки: Москва, Внуково, аэропорт
Прибытие: Баку
Время отправления: ххххх

*/

/*
Готовый способ добраться из одного города в другой
Он в себе хранит вектор возможных пересадок
*/
struct Route {
  std::string departure_city_;
  std::string departure_time_;

  std::string arrival_city_;
  std::string arrival_time_;

  std::vector<Segment> segments_;
};

struct RouteRequest {
 public:
  RouteRequest() {}
  RouteRequest(const std::string& from, const std::string& to,
               const std::string& date);

  std::string departure_city_;
  std::string arrival_city_;

  std::string date_;

  std::optional<std::string> transport_type_;
  
  std::string has_transfers_ = "false";
};