#include <gtest/gtest.h>

#include <stations_parser.hpp>

using json = nlohmann::json

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

}  // namespace