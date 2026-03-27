cmake_minimum_required(VERSION 3.23)

project(labwork7 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG c492b4fed1fd7288eacec6fe8d89e37df78d373a
)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.16.0/googletest-1.16.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)

enable_testing()

add_subdirectory(lib)
add_subdirectory(tests)

add_executable(labwork7 main.cpp)

target_link_libraries(labwork7 PRIVATE
  nlohmann_json::nlohmann_json
  cpr::cpr
  business_logic
  cache_repo
  repos
  stations_parser
  presentation
)

include(tests/coverage.cmake)