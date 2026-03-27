add_executable(labwork7_tests
  stations_parser_test.cpp
  cache_repository_test.cpp
  cache_test.cpp
  json_work_test.cpp
  main_service_test.cpp
  presentation_test.cpp
)

target_link_libraries(labwork7_tests PRIVATE
  GTest::gtest_main
  GTest::gmock
  nlohmann_json::nlohmann_json
  cpr::cpr
  business_logic
  cache_repo
  repos
  stations_parser
  presentation
)

target_include_directories(labwork7_tests PRIVATE
  ${PROJECT_SOURCE_DIR}
)

include(GoogleTest)
gtest_discover_tests(labwork7_tests)