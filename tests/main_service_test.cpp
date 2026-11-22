#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <business_logic.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <structs.hpp>
#include <unordered_map>
#include <vector>

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::Throw;

namespace {

struct MockRouteRepository : IRouteRepository {
  MOCK_METHOD(std::optional<std::vector<Route>>, GetRoute,
              (const RouteRequest& request), (override));
};

struct MockCitiesDict : ICitiesDict {
  MOCK_METHOD((std::unordered_map<std::string, std::string>), MakeCitiesDict,
              (), (override));
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

TEST(MainServiceTest, FilterWithoutTransfers) {
  MockRouteRepository repo;
  MockCitiesDict cities;
  MainService service(repo, cities);

  RouteRequest request{"Уфа", "Санкт-Петербург", "2026-03-30"};
  request.has_transfers_ = "false";

  EXPECT_CALL(cities, MakeCitiesDict())
      .WillOnce(Return(std::unordered_map<std::string, std::string>{
          {"Уфа", "c172"}, {"Санкт-Петербург", "c2"}}));

  EXPECT_CALL(repo, GetRoute(_))
      .WillOnce(Return(std::optional<std::vector<Route>>{
          std::vector<Route>{MakeRoute(false), MakeRoute(true)}}));

  auto result = service.ProcessRequest(request);
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1);
  EXPECT_FALSE(result->front().transfer_info.has_value());
}

TEST(MainServiceTest, FiltersByTransportTypeAcrossSegments) {
  MockRouteRepository repo;
  MockCitiesDict cities;
  MainService service(repo, cities);

  RouteRequest request{"Уфа", "Санкт-Петербург", "2026-03-30"};
  request.transport_type_ = "plane";

  EXPECT_CALL(cities, MakeCitiesDict())
      .WillOnce(Return(std::unordered_map<std::string, std::string>{
          {"Уфа", "c172"}, {"Санкт-Петербург", "c2"}}));

  EXPECT_CALL(repo, GetRoute(_))
      .WillOnce(Return(std::optional<std::vector<Route>>{
          std::vector<Route>{MakeRoute(true)}}));

  auto result = service.ProcessRequest(request);
  ASSERT_TRUE(result.has_value());
}

TEST(MainServiceTest, ThrowWhenNoCityFound) {
  MockRouteRepository repo;
  MockCitiesDict cities;
  MainService service(repo, cities);

  RouteRequest request{"Уфа", "Санкт-Петербург", "2026-03-30"};

  EXPECT_CALL(cities, MakeCitiesDict())
      .WillOnce(Return(
          std::unordered_map<std::string, std::string>{{"Уфа", "c172"}}));

  EXPECT_CALL(repo, GetRoute(_)).Times(0);

  EXPECT_THROW(service.ProcessRequest(request), std::runtime_error);
}

TEST(MainServiceTest, RethrowsRepositoryErrorsWithContext) {
  MockRouteRepository repo;
  MockCitiesDict cities;
  MainService service(repo, cities);

  RouteRequest request{"Уфа", "Санкт-Петербург", "2026-03-30"};

  EXPECT_CALL(cities, MakeCitiesDict())
      .WillOnce(Return(std::unordered_map<std::string, std::string>{
          {"Уфа", "c172"}, {"Санкт-Петербург", "c2"}}));

  EXPECT_CALL(repo, GetRoute(_))
      .WillOnce(Throw(std::runtime_error("exception")));

  try {
    service.ProcessRequest(request);
    FAIL() << "Expected exception";
  } catch (const std::runtime_error& e) {
    EXPECT_THAT(std::string{e.what()},
                ::testing::HasSubstr("Failed to get routes"));
    EXPECT_THAT(std::string{e.what()}, ::testing::HasSubstr("exception"));
  }
}


class RouteDurationTest : public ::testing::Test {
 protected:
  MockRouteRepository repo;
  MockCitiesDict cities;
  MainService service{repo, cities};
  RouteRequest request{"Уфа", "Санкт-Петербург", "2026-03-30"};

  void SetUp() override {
    request.has_transfers_ = "true";
    ON_CALL(cities, MakeCitiesDict())
        .WillByDefault(Return(std::unordered_map<std::string, std::string>{
            {"Уфа", "c172"}, {"Санкт-Петербург", "c2"}}));
  }

  void ReturnRoutes(const std::vector<Route>& routes) {
    EXPECT_CALL(repo, GetRoute(_)).WillOnce(Return(routes));
  }
};

TEST_F(RouteDurationTest, IncludesAllSegmentsAndTransferWaitingTime) {
  EXPECT_EQ(MakeRoute(true).DurationSeconds(), 22800);
  EXPECT_EQ(MakeRoute(false).DurationSeconds(), 13800);
  EXPECT_EQ(Route{}.DurationSeconds(), 0);
}

TEST_F(RouteDurationTest, FiltersAtInclusiveDurationBoundary) {
  request.max_duration_seconds_ = 13800;
  ReturnRoutes({MakeRoute(true), MakeRoute(false)});
  auto result = service.ProcessRequest(request);
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1);
  EXPECT_EQ(result->front().DurationSeconds(), 13800);
}

TEST_F(RouteDurationTest, ReturnsEmptyWhenEveryRouteExceedsLimit) {
  request.max_duration_seconds_ = 1;
  ReturnRoutes({MakeRoute(true), MakeRoute(false)});
  auto result = service.ProcessRequest(request);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(result->empty());
}

TEST_F(RouteDurationTest, SortsByTotalDurationAndPreservesTies) {
  auto first = MakeRoute(false);
  first.departure_city_title = "first";
  auto second = first;
  second.departure_city_title = "second";
  request.sort_by_duration_ = true;
  ReturnRoutes({MakeRoute(true), first, second});
  auto result = service.ProcessRequest(request);
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 3);
  EXPECT_EQ((*result)[0].departure_city_title, "first");
  EXPECT_EQ((*result)[1].departure_city_title, "second");
  EXPECT_EQ((*result)[2].DurationSeconds(), 22800);
}

TEST_F(RouteDurationTest, KeepsRepositoryOrderByDefault) {
  ReturnRoutes({MakeRoute(true), MakeRoute(false)});
  auto result = service.ProcessRequest(request);
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2);
  EXPECT_EQ(result->front().DurationSeconds(), 22800);
}

TEST_F(RouteDurationTest, CombinesDurationAndTransportFilters) {
  auto train = MakeRoute(false);
  for (auto& segment : train.segments) {
    segment.transport_type = "train";
  }
  request.max_duration_seconds_ = 20000;
  request.transport_type_ = "plane";
  request.sort_by_duration_ = true;
  ReturnRoutes({MakeRoute(true), train, MakeRoute(false)});
  auto result = service.ProcessRequest(request);
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1);
  EXPECT_EQ(result->front().segments.front().transport_type, "plane");
}

TEST_F(RouteDurationTest, PreservesMissingRepositoryResult) {
  request.sort_by_duration_ = true;
  EXPECT_CALL(repo, GetRoute(_)).WillOnce(Return(std::nullopt));
  EXPECT_FALSE(service.ProcessRequest(request).has_value());
}

TEST_F(RouteDurationTest, RejectsNonPositiveLimitsBeforeRepositoryAccess) {
  EXPECT_CALL(cities, MakeCitiesDict()).Times(0);
  EXPECT_CALL(repo, GetRoute(_)).Times(0);
  for (long long limit : {0LL, -1LL}) {
    request.max_duration_seconds_ = limit;
    EXPECT_THROW(service.ProcessRequest(request), std::invalid_argument);
  }
}

TEST_F(RouteDurationTest, SumsDurationsBeyondIntegerRange) {
  Route route;
  Segment segment;
  segment.duration_seconds = 2000000000;
  route.segments = {segment, segment};
  route.transfer_info = TransferInfo{"", "", "", 2000000000};
  EXPECT_EQ(route.DurationSeconds(), 6000000000LL);
}

}  // namespace