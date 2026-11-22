#include <gtest/gtest.h>

#include <cstdlib>
#include <iostream>
#include <optional>
#include <presentation.hpp>
#include <sstream>
#include <streambuf>
#include <string>
#include <structs.hpp>
#include <unistd.h>
#include <vector>

namespace {

Route MakeRoute() {
  Route route;
  route.departure_city_title = "Уфа";
  route.arrival_city_title = "Санкт-Петербург";
  route.date_ = "2026-03-30";
  route.transfer_info =
      TransferInfo{"Москва", "Шереметьево", "Шереметьево", 9000};

  route.segments.push_back(
      Segment{"Уфа", "Уфа", "2026-03-30T19:35:00+05:00", "Москва", "Москва",
              "2026-03-30T20:00:00+03:00", "plane", "Аэрофлот", 8700});

  route.segments.push_back(Segment{
      "Москва", "Москва", "2026-03-30T22:30:00+03:00", "Санкт-Петербург",
      "Санкт-Петербург", "2026-03-30T23:55:00+03:00", "plane", "Россия", 5100});

  return route;
}

TEST(PresentationTest, ACoupleOfWrongInputs) {
  std::istringstream user_input(
      "\nУфа\nСанкт-Петербург\n20260330\n2026-03-"
      "30\nplane\nlol\nkek\ncheburek\nyes\n\n\n");
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
  EXPECT_FALSE(request.max_duration_seconds_.has_value());
  EXPECT_FALSE(request.sort_by_duration_);
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
  EXPECT_NE(out.find("Общее время в пути: 6 ч 20 мин"), std::string::npos);
}

TEST(PresentationTest, ReadsDurationOptionsAndRejectsInvalidInput) {
  std::istringstream input(
      "Уфа\nСанкт-Петербург\n2026-03-30\n\nyes\n"
      "0\n-1\n1.5\n30abc\n9223372036854775807\n180\nmaybe\nyes\n");
  std::ostringstream output;
  auto* original_in = std::cin.rdbuf(input.rdbuf());
  auto* original_out = std::cout.rdbuf(output.rdbuf());

  RouteRequest request = CLIParser{}.ReadInteractiveRequest();

  std::cin.rdbuf(original_in);
  std::cout.rdbuf(original_out);
  ASSERT_TRUE(request.max_duration_seconds_.has_value());
  EXPECT_EQ(*request.max_duration_seconds_, 10800);
  EXPECT_TRUE(request.sort_by_duration_);
}

TEST(PresentationTest, ThrowsWhenDurationInputEnds) {
  std::istringstream input("Уфа\nСанкт-Петербург\n2026-03-30\n\nno\n");
  std::ostringstream output;
  auto* original_in = std::cin.rdbuf(input.rdbuf());
  auto* original_out = std::cout.rdbuf(output.rdbuf());

  EXPECT_THROW(CLIParser{}.ReadInteractiveRequest(), std::runtime_error);

  std::cin.rdbuf(original_in);
  std::cin.clear();
  std::cout.rdbuf(original_out);
}

TEST(PresentationTest, ThrowsWhenDepartureCityInputEnds) {
  class NullBuffer : public std::streambuf {
    int overflow(int character) override {
      return traits_type::not_eof(character);
    }
  };

  ASSERT_EXIT(
      {
        std::istringstream input;
        NullBuffer output;
        std::cin.rdbuf(input.rdbuf());
        std::cin.clear();
        std::cout.rdbuf(&output);
        alarm(1);

        try {
          CLIParser{}.ReadInteractiveRequest();
        } catch (const std::runtime_error& error) {
          std::exit(std::string(error.what()) == "Ввод прерван." ? 0 : 2);
        }
        std::exit(3);
      },
      ::testing::ExitedWithCode(0), "");
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
