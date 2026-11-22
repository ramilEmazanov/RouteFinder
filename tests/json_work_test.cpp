#include <gtest/gtest.h>

#include <json_work.hpp>
#include <nlohmann/json_fwd.hpp>
#include <structs.hpp>
#include <stdexcept>

using json = nlohmann::json;

namespace {

TEST(JsonWorkTest, DirectUfaKazan) {
  const json data = {
      {"interval_segments", json::array()},
      {"pagination", {{"limit", 100}, {"offset", 0}, {"total", 1}}},
      {"search",
       {{"date", "2026-03-23"},
        {"from",
         {{"code", "c172"},
          {"popular_title", "Уфа"},
          {"short_title", "Уфа"},
          {"title", "Уфа"},
          {"type", "settlement"}}},
        {"to",
         {{"code", "c43"},
          {"popular_title", "Казань"},
          {"short_title", "Казань"},
          {"title", "Казань"},
          {"type", "settlement"}}}}},
      {"segments",
       json::array(
           {{{"arrival", "2026-03-23T07:00:00+03:00"},
             {"arrival_platform", ""},
             {"arrival_terminal", nullptr},
             {"departure", "2026-03-23T01:00:00+05:00"},
             {"departure_platform", ""},
             {"departure_terminal", nullptr},
             {"duration", 28800.0},
             {"from",
              {{"code", "s9636684"},
               {"popular_title", nullptr},
               {"short_title", nullptr},
               {"station_type", "bus_station"},
               {"station_type_name", "автовокзал"},
               {"title", "Уфа, Южный Автовокзал"},
               {"transport_type", "bus"},
               {"type", "station"}}},
             {"has_transfers", false},
             {"start_date", "2026-03-23"},
             {"stops", ""},
             {"thread",
              {{"carrier",
                {{"address", ""},
                 {"code", 67384},
                 {"codes",
                  {{"iata", nullptr}, {"icao", nullptr}, {"sirena", nullptr}}},
                 {"contacts", ""},
                 {"email", ""},
                 {"logo", nullptr},
                 {"logo_svg", nullptr},
                 {"phone", ""},
                 {"title",
                  "Общество с Ограниченной Ответственностью \"Транспортный "
                  "Мир\""},
                 {"url", ""}}},
               {"express_type", nullptr},
               {"number", ""},
               {"short_title", "Уфа, Южный Автовокзал — Казань"},
               {"thread_method_link",
                "api.rasp.yandex-net.ru/v3/thread/"
                "?date=2026-03-23&uid=empty_0_f9636684t9850085_168"},
               {"title", "Уфа, Южный Автовокзал — Казань"},
               {"transport_subtype",
                {{"code", nullptr}, {"color", nullptr}, {"title", nullptr}}},
               {"transport_type", "bus"},
               {"uid", "empty_0_f9636684t9850085_168"},
               {"vehicle", "KING LONG  (51), м753мс702"}}},
             {"tickets_info", nullptr},
             {"to",
              {{"code", "s9850085"},
               {"popular_title", "Восточный автовокзал"},
               {"short_title", ""},
               {"station_type", "bus_station"},
               {"station_type_name", "автовокзал"},
               {"title", "Казань, Восточный автовокзал"},
               {"transport_type", "bus"},
               {"type", "station"}}}}})}};

  RouteRequest request{"c172", "c43", "2026-03-23"};
  auto result = APIJsonExtracter{}.Extract(data, request);

  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1);

  const Route& route = (*result)[0];
  EXPECT_EQ(route.departure_city_title, "Уфа");
  EXPECT_EQ(route.arrival_city_title, "Казань");
  EXPECT_EQ(route.date_, "2026-03-23");
  EXPECT_FALSE(route.transfer_info.has_value());

  ASSERT_EQ(route.segments.size(), 1);

  const Segment& segment = route.segments[0];
  EXPECT_EQ(segment.departure_city_title, "Уфа");
  EXPECT_EQ(segment.departure_station_title, "Уфа, Южный Автовокзал");
  EXPECT_EQ(segment.departure_time, "2026-03-23T01:00:00+05:00");

  EXPECT_EQ(segment.arrival_city_title, "Казань");
  EXPECT_EQ(segment.arrival_station_title, "Казань, Восточный автовокзал");
  EXPECT_EQ(segment.arrival_time, "2026-03-23T07:00:00+03:00");

  EXPECT_EQ(segment.transport_type, "bus");
  EXPECT_EQ(segment.carrier_title,
            "Общество с Ограниченной Ответственностью \"Транспортный Мир\"");
  EXPECT_EQ(segment.duration_seconds, 28800);
}

TEST(JsonWorkTest, NulloptWhenNoRoutes) {
  const json data = {{"pagination", {{"total", 0}}},
                     {"segments", json::array()}};

  RouteRequest request{"c172", "c43", "2026-03-23"};
  auto result = APIJsonExtracter{}.Extract(data, request);
  EXPECT_FALSE(result.has_value());
}

TEST(JsonWorkTest, ThrowsOnInvalidJsonStructure) {
  const json data = {{"segments", json::array()}};

  RouteRequest request{"c172", "c43", "2026-03-23"};
  EXPECT_THROW(APIJsonExtracter{}.Extract(data, request), std::runtime_error);
}

TEST(JsonWorkTest, UfaBakuTransfer) {
  const json data = {
      {"interval_segments", json::array()},
      {"pagination", {{"limit", 100}, {"offset", 0}, {"total", 1}}},
      {"search",
       {{"date", "2026-05-24"},
        {"from",
         {{"code", "c172"},
          {"popular_title", "Уфа"},
          {"short_title", "Уфа"},
          {"title", "Уфа"},
          {"type", "settlement"}}},
        {"to",
         {{"code", "c10253"},
          {"popular_title", "Баку"},
          {"short_title", "Баку"},
          {"title", "Баку"},
          {"type", "settlement"}}}}},
      {"segments",
       json::array(
           {{{"arrival", "2026-05-24T14:05:00+04:00"},
             {"arrival_to",
              {{"code", "s9600387"},
               {"popular_title", ""},
               {"short_title", ""},
               {"station_type", "airport"},
               {"station_type_name", "аэропорт"},
               {"title", "им. Гейдара Алиева"},
               {"transport_type", "plane"},
               {"type", "station"}}},
             {"departure", "2026-05-24T07:00:00+05:00"},
             {"departure_from",
              {{"code", "s9600393"},
               {"popular_title", ""},
               {"short_title", ""},
               {"station_type", "airport"},
               {"station_type_name", "аэропорт"},
               {"title", "Уфа"},
               {"transport_type", "plane"},
               {"type", "station"}}},
             {"details",
              json::array(
                  {{{"arrival", "2026-05-24T07:15:00+03:00"},
                    {"arrival_platform", ""},
                    {"arrival_terminal", "A"},
                    {"departure", "2026-05-24T07:00:00+05:00"},
                    {"departure_platform", ""},
                    {"departure_terminal", "1"},
                    {"duration", 8100.0},
                    {"from",
                     {{"code", "c172"},
                      {"popular_title", "Уфа"},
                      {"short_title", "Уфа"},
                      {"title", "Уфа"},
                      {"type", "settlement"}}},
                    {"start_date", "2026-05-24"},
                    {"stops", ""},
                    {"thread",
                     {{"carrier",
                       {{"code", 9144},
                        {"codes",
                         {{"iata", "DP"}, {"icao", nullptr}, {"sirena", "ДР"}}},
                        {"title", "Победа"}}},
                      {"express_type", nullptr},
                      {"number", "DP 436"},
                      {"short_title", "Уфа — Москва"},
                      {"title", "Уфа — Москва"},
                      {"transport_subtype",
                       {{"code", nullptr},
                        {"color", nullptr},
                        {"title", nullptr}}},
                      {"transport_type", "plane"},
                      {"uid", "DP-436_260524_c9144_12"},
                      {"vehicle", "Boeing 737-800"}}},
                    {"to",
                     {{"code", "c213"},
                      {"popular_title", "Москва"},
                      {"short_title", "Москва"},
                      {"title", "Москва"},
                      {"type", "settlement"}}}},
                   {{"duration", 9300.0},
                    {"is_transfer", true},
                    {"transfer_from",
                     {{"code", "s9600215"},
                      {"popular_title", nullptr},
                      {"short_title", nullptr},
                      {"station_type", "airport"},
                      {"station_type_name", "аэропорт"},
                      {"title", "Внуково"},
                      {"transport_type", "plane"},
                      {"type", "station"}}},
                    {"transfer_point",
                     {{"code", "c213"},
                      {"popular_title", "Москва"},
                      {"short_title", "Москва"},
                      {"title", "Москва"},
                      {"type", "settlement"}}},
                    {"transfer_to",
                     {{"code", "s9600215"},
                      {"popular_title", nullptr},
                      {"short_title", nullptr},
                      {"station_type", "airport"},
                      {"station_type_name", "аэропорт"},
                      {"title", "Внуково"},
                      {"transport_type", "plane"},
                      {"type", "station"}}}},
                   {{"arrival", "2026-05-24T14:05:00+04:00"},
                    {"arrival_platform", ""},
                    {"arrival_terminal", "1"},
                    {"departure", "2026-05-24T09:50:00+03:00"},
                    {"departure_platform", ""},
                    {"departure_terminal", "A"},
                    {"duration", 11700.0},
                    {"from",
                     {{"code", "c213"},
                      {"popular_title", "Москва"},
                      {"short_title", "Москва"},
                      {"title", "Москва"},
                      {"type", "settlement"}}},
                    {"start_date", "2026-05-24"},
                    {"stops", ""},
                    {"thread",
                     {{"carrier",
                       {{"code", 29},
                        {"codes",
                         {{"iata", "UT"}, {"icao", "UTA"}, {"sirena", "ЮТ"}}},
                        {"title", "ЮТэйр"}}},
                      {"express_type", nullptr},
                      {"number", "UT 745"},
                      {"short_title", "Москва — Баку"},
                      {"title", "Москва — Баку"},
                      {"transport_subtype",
                       {{"code", nullptr},
                        {"color", nullptr},
                        {"title", nullptr}}},
                      {"transport_type", "plane"},
                      {"uid", "UT-745_260524_c29_12"},
                      {"vehicle", "Boeing 737-800"}}},
                    {"to",
                     {{"code", "c10253"},
                      {"popular_title", "Баку"},
                      {"short_title", "Баку"},
                      {"title", "Баку"},
                      {"type", "settlement"}}}}})},
             {"has_transfers", true},
             {"transfers", json::array({{{"code", "c213"},
                                         {"popular_title", "Москва"},
                                         {"short_title", "Москва"},
                                         {"title", "Москва"},
                                         {"type", "settlement"}}})},
             {"transport_types", json::array({"plane", "plane"})}}})}};

  RouteRequest request{"c172", "c10253", "2026-05-24"};
  auto result = APIJsonExtracter{}.Extract(data, request);

  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1);

  const Route& route = (*result)[0];
  EXPECT_EQ(route.departure_city_title, "Уфа");
  EXPECT_EQ(route.arrival_city_title, "Баку");
  EXPECT_EQ(route.date_, "2026-05-24");

  ASSERT_TRUE(route.transfer_info.has_value());
  EXPECT_EQ(route.transfer_info->city_title, "Москва");
  EXPECT_EQ(route.transfer_info->transfer_from_station_title, "Внуково");
  EXPECT_EQ(route.transfer_info->transfer_to_station_title, "Внуково");
  EXPECT_EQ(route.transfer_info->duration_seconds, 9300);

  ASSERT_EQ(route.segments.size(), 2);

  const Segment& first = route.segments[0];
  EXPECT_EQ(first.departure_city_title, "Уфа");
  EXPECT_EQ(first.arrival_city_title, "Москва");
  EXPECT_EQ(first.departure_time, "2026-05-24T07:00:00+05:00");
  EXPECT_EQ(first.arrival_time, "2026-05-24T07:15:00+03:00");
  EXPECT_EQ(first.transport_type, "plane");
  EXPECT_EQ(first.carrier_title, "Победа");
  EXPECT_EQ(first.duration_seconds, 8100);

  const Segment& second = route.segments[1];
  EXPECT_EQ(second.departure_city_title, "Москва");
  EXPECT_EQ(second.arrival_city_title, "Баку");
  EXPECT_EQ(second.departure_time, "2026-05-24T09:50:00+03:00");
  EXPECT_EQ(second.arrival_time, "2026-05-24T14:05:00+04:00");
  EXPECT_EQ(second.transport_type, "plane");
  EXPECT_EQ(second.carrier_title, "ЮТэйр");
  EXPECT_EQ(second.duration_seconds, 11700);
}

}  // namespace