#include <gtest/gtest.h>

#include <cache.hpp>
#include <chrono>
#include <nlohmann/json.hpp>
#include <string>

#include <filesystem>
#include <fstream>
#include <structs.hpp>
#include <vector>

using json = nlohmann::json;

namespace {

Route MakeRoute(bool with_transfer = true) {
  Route route;
  route.departure_city_title = "Уфа";
  route.arrival_city_title = "Санкт-Петербург";
  route.date_ = "2026-03-30";

  route.segments.push_back(
      Segment{"Уфа", "Уфа", "2026-03-30T19:35:00+05:00", "Москва", "Москва",
              "2026-03-30T20:00:00+03:00", "plane", "Аэрофлот", 8700});

  route.segments.push_back(Segment{
      "Москва", "Москва", "2026-03-30T22:30:00+03:00", "Санкт-Петербург",
      "Санкт-Петербург", "2026-03-30T23:55:00+03:00", "plane", "Россия", 5100});

  if (with_transfer) {
    route.transfer_info =
        TransferInfo{"Москва", "Шереметьево", "Шереметьево", 9000};
  }

  return route;
}

std::string MakeKey(const RouteRequest& request) {
  return request.departure_city_ + "|" + request.arrival_city_ + "|" +
         request.date_ + "|" +
         (request.transport_type_.has_value() ? *request.transport_type_
                                              : "null") +
         "|" + request.has_transfers_;
}

class CacheTest : public ::testing::Test {
 protected:
  void SetUp() override {
    path_ = std::filesystem::temp_directory_path() / "labwork7_cache_test.json";
    std::filesystem::remove(path_);
  }

  void TearDown() override { std::filesystem::remove(path_); }

  std::filesystem::path path_;
};

TEST_F(CacheTest, WriteAndExtract) {
  Cache cache(path_.string(), 10);
  RouteRequest request{"c172", "c2", "2026-03-30"};
  request.transport_type_ = "plane";
  request.has_transfers_ = "true";

  cache.WriteToCache(request, std::vector<Route>{MakeRoute()});
  auto result = cache.Extract(request);

  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1);
  ASSERT_EQ(result->front().segments.size(), 2);
  EXPECT_EQ(result->front().segments.front().transport_type, "plane");
  EXPECT_TRUE(result->front().transfer_info.has_value());
}

TEST_F(CacheTest, MissingKey) {
  Cache cache(path_.string(), 10);
  RouteRequest request{"c172", "c2", "2026-03-30"};

  auto result = cache.Extract(request);
  EXPECT_FALSE(result.has_value());
}

TEST_F(CacheTest, RemoveExpired) {
  using namespace std::chrono;
  const auto now =
      duration_cast<seconds>(system_clock::now().time_since_epoch()).count();

  RouteRequest old_request{"old_from", "old_to", "2026-03-30"};
  RouteRequest fresh_request{"fresh_from", "fresh_to", "2026-03-30"};

  json j;
  j[MakeKey(old_request)] = {{"request_unix_time", now - 7200},
                             {"routes", std::vector<Route>{MakeRoute("old")}}};
  j[MakeKey(fresh_request)] = {
      {"request_unix_time", now},
      {"routes", std::vector<Route>{MakeRoute("fresh")}}};

  std::ofstream(path_) << j.dump(2);

  Cache cache(path_, 10);

  EXPECT_FALSE(cache.Extract(old_request).has_value());

  auto fresh = cache.Extract(fresh_request);
  ASSERT_TRUE(fresh.has_value());
  ASSERT_EQ(fresh->front().segments.size(), 2);
  EXPECT_EQ(fresh->front().segments.front().transport_type, "plane");
  EXPECT_EQ(fresh->front().segments.back().transport_type, "plane");
}

TEST_F(CacheTest, KeepingSizeConstant) {
  using namespace std::chrono;
  auto now =
      duration_cast<seconds>(system_clock::now().time_since_epoch()).count();

  json j;
  for (int i = 0; i < 3; ++i) {
    RouteRequest req{"from" + std::to_string(i), "to" + std::to_string(i),
                     "2026-03-30"};
    j[MakeKey(req)] = {{"request_unix_time", now + i},
                       {"routes", std::vector<Route>{MakeRoute()}}};
  }
  std::ofstream(path_) << j.dump(2);

  Cache cache(path_, 2);

  int found = 0;
  for (int i = 0; i < 3; ++i) {
    RouteRequest req{"from" + std::to_string(i), "to" + std::to_string(i),
                     "2026-03-30"};
    found += cache.Extract(req).has_value() ? 1 : 0;
  }
  EXPECT_EQ(found, 2);
}

}  // namespace