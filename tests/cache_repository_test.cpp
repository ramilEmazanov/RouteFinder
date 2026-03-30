#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <business_logic.hpp>
#include <cache.hpp>
#include <filesystem>
#include <optional>
#include <repos.hpp>
#include <structs.hpp>
#include <vector>

using ::testing::_;
using ::testing::Return;
using ::testing::Throw;

namespace {

struct MockRouteRepository : IRouteRepository {
  MOCK_METHOD(std::optional<std::vector<Route>>, GetRoute,
              (const RouteRequest& request), (override));
};

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

class CacheRepositoryTest : public ::testing::Test {
 protected:
  void SetUp() override {
    path_ = std::filesystem::temp_directory_path() /
            "labwork7_cache_repo_test.json";
    std::filesystem::remove(path_);
  }

  void TearDown() override { std::filesystem::remove(path_); }

  std::filesystem::path path_;
};

TEST_F(CacheRepositoryTest, NoApiCalling) {
  Cache cache(path_.string(), 10);
  RouteRequest request{"c172", "c2", "2026-03-30"};
  auto routes =
      std::optional<std::vector<Route>>{std::vector<Route>{MakeRoute()}};
  cache.WriteToCache(request, routes);

  MockRouteRepository api;
  CacheRepository repo(api, cache);

  EXPECT_CALL(api, GetRoute(_)).Times(0);

  auto result = repo.GetRoute(request);
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1);
  ASSERT_EQ(result->front().segments.size(), 2);
  EXPECT_EQ(result->front().departure_city_title, "Уфа");
  EXPECT_EQ(result->front().arrival_city_title, "Санкт-Петербург");
}

TEST_F(CacheRepositoryTest, APIcall) {
  Cache cache(path_.string(), 10);
  RouteRequest request{"c172", "c2", "2026-03-30"};

  MockRouteRepository api;
  CacheRepository repo(api, cache);

  EXPECT_CALL(api, GetRoute(_))
      .Times(1)
      .WillOnce(Return(std::vector<Route>{MakeRoute()}));

  auto first = repo.GetRoute(request);
  ASSERT_TRUE(first.has_value());
  ASSERT_EQ(first->size(), 1);

  EXPECT_CALL(api, GetRoute(_)).Times(0);

  auto second = repo.GetRoute(request);
  ASSERT_TRUE(second.has_value());
  ASSERT_EQ(second->size(), 1);
  ASSERT_EQ(second->front().segments.size(), 2);
  EXPECT_TRUE(second->front().transfer_info.has_value());
}

TEST_F(CacheRepositoryTest, NulloptWhenAPIThrows) {
  Cache cache(path_.string(), 10);
  RouteRequest request{"c172", "c2", "2026-03-30"};

  MockRouteRepository api;
  CacheRepository repo(api, cache);

  EXPECT_CALL(api, GetRoute(_)).WillOnce(Throw(std::runtime_error("net fail")));

  auto result = repo.GetRoute(request);
  EXPECT_FALSE(result.has_value());
}

}  // namespace