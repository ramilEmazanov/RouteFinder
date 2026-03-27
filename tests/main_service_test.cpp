#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <business_logic.hpp>

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

  EXPECT_CALL(repo, GetRoute(_)).WillOnce(Throw(std::runtime_error("exception")));

  try {
    service.ProcessRequest(request);
    FAIL() << "Expected exception";
  } catch (const std::runtime_error& e) {
    EXPECT_THAT(std::string{e.what()},
                ::testing::HasSubstr("Failed to get routes"));
    EXPECT_THAT(std::string{e.what()}, ::testing::HasSubstr("exception"));
  }
}

}  // namespace