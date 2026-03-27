#include <gtest/gtest.h>
#include <presentation.hpp>
#include <sstream>

namespace {

Route MakeRoute() {
  Route route;
  route.departure_city_title = "Уфа";
  route.arrival_city_title = "Санкт-Петербург";
  route.date_ = "2026-03-30";
  route.transfer_info =
      TransferInfo{"Москва", "Шереметьево", "Шереметьево", 9000};

  route.segments.push_back(
      Segment{"Уфа", "Уфа", "2026-03-30T19:35:00+05:00",
              "Москва", "Москва", "2026-03-30T20:00:00+03:00",
              "plane", "Аэрофлот", 8700});

  route.segments.push_back(
      Segment{"Москва", "Москва", "2026-03-30T22:30:00+03:00",
              "Санкт-Петербург", "Санкт-Петербург",
              "2026-03-30T23:55:00+03:00",
              "plane", "Россия", 5100});

  return route;
}

TEST(PresentationTest, ACoupleOfWrongInputs) {
  std::istringstream user_input(
      "\nУфа\nСанкт-Петербург\n20260330\n2026-03-30\nplane\nlol\nkek\ncheburek\nyes\n");
  std::ostringstream output;

  auto* default_in_buffer = std::cin.rdbuf(user_input.rdbuf());
  auto* default_out_buffer = std::cout.rdbuf(output.rdbuf());

  CLIParser parser;
  RouteRequest request = parser.ReadInteractiveRequest();

  std::cin.rdbuf(default_in_buffer);
  std::cout.rdbuf(default_out_buffer);

  EXPECT_EQ(request.departure_city_, "Уфа");
  EXPECT_EQ(request.arrival_city_, "Санкт-Петербург");
  EXPECT_EQ(request.date_, "2026-03-30");
  ASSERT_TRUE(request.transport_type_.has_value());
  EXPECT_EQ(*request.transport_type_, "plane");
  EXPECT_EQ(request.has_transfers_, "true");
}

TEST(PresentationTest, PrintRoutesPrintsHumanReadableDetails) {
  std::ostringstream output;
  auto* default_out_buffer = std::cout.rdbuf(output.rdbuf());

  RoutePrinter printer;
  printer.PrintRoutes(std::vector<Route>{MakeRoute()});

  std::cout.rdbuf(default_out_buffer);

  std::string out = output.str();
  EXPECT_NE(out.find("Путь №1"), std::string::npos);
  EXPECT_NE(out.find("Количество пересадок: 1"), std::string::npos);
  EXPECT_NE(out.find("Длительность пересадки: 2 ч 30 мин"), std::string::npos);
  EXPECT_NE(out.find("Тип транспорта: Самолет"), std::string::npos);
}

TEST(PresentationTest, PrintRoutesPrintsNoRoutesForNullopt) {
  std::ostringstream output;
  auto* default_out_buffer = std::cout.rdbuf(output.rdbuf());

  RoutePrinter printer;
  printer.PrintRoutes(std::nullopt);

  std::cout.rdbuf(default_out_buffer);

  EXPECT_NE(output.str().find("No routes found"), std::string::npos);
}

}  // namespace