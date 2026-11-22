#include <gtest/gtest.h>

#include <stations_parser.hpp>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

namespace {

TEST(StationsParserTest, FromRawApiJSON) {
  json raw = {
      {"countries",
       nlohmann::json::array({{{"regions",
                                nlohmann::json::array({{{"settlements",
                                                         nlohmann::json::array(
                                                             {{{"title", "Санкт-Петербург"},
                                                               {"codes", {{"yandex_code", "c2"}}}},
                                                              {{"title", "Казань"},
                                                               {"codes", {{"yandex_code", "c43"}}}}})}}})}}})}};

  auto clean = CitiesDictMaker{}.BuildCitiesDict(raw);
  ASSERT_TRUE(clean.contains("request_unix_time"));
  ASSERT_TRUE(clean.contains("cities"));
  EXPECT_EQ(clean["cities"]["Санкт-Петербург"], "c2");
  EXPECT_EQ(clean["cities"]["Казань"], "c43");
}

TEST(StationsParserTest, CleanJSONIntoUnorderedMap) {
  json clean = {{"request_unix_time", 12345},
                                {"cities", {{"Санкт-Петербург", "c2"}, {"Казань", "c43"}}}};

  auto result = JsonToUnorderedMapMaker{}.TurnJsonToMap(clean);
  ASSERT_EQ(result.size(), 2);
  EXPECT_EQ(result["Санкт-Петербург"], "c2");
  EXPECT_EQ(result["Казань"], "c43");
}

TEST(StationsParserTest, ThrowsWhenCitiesObjectMissing) {
  json invalid = {{"request_unix_time", 12345}};
  EXPECT_THROW(JsonToUnorderedMapMaker{}.TurnJsonToMap(invalid), std::runtime_error);
}

TEST(StationsParserTest, DictSkipWithoutRegions) {
  json raw = {
      {"countries", json::array({
          {{"title", "Country1"}}
      })}
  };

  auto clean = CitiesDictMaker{}.BuildCitiesDict(raw);

  ASSERT_TRUE(clean.contains("cities"));
  EXPECT_TRUE(clean["cities"].empty());
}

TEST(StationsParserTest, DictSkipsWithoutSettlements) {
  json raw = {
      {"countries", json::array({
          {{"regions", json::array({
              {{"title", "Region1"}}
          })}}
      })}
  };

  auto clean = CitiesDictMaker{}.BuildCitiesDict(raw);

  ASSERT_TRUE(clean.contains("cities"));
  EXPECT_TRUE(clean["cities"].empty());
}

TEST(StationsParserTest, WriteCleanCitiesDict) {
  std::filesystem::path filename = PathHelper{}.DictPath();
  std::filesystem::remove(filename);

  json clean = {
      {"request_unix_time", 12345},
      {"cities", {{"Санкт-Петербург", "c2"}}}
  };

  ASSERT_NO_THROW(CleanJsonCitiesDictWriter{}.WriteCleanCitiesDict(clean));

  std::ifstream in(filename);
  ASSERT_TRUE(in.is_open());

  json read_back = json::parse(in);
  EXPECT_EQ(read_back["cities"]["Санкт-Петербург"], "c2");

  std::filesystem::remove(filename);
}

}  // namespace