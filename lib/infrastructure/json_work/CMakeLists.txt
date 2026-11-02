cmake_minimum_required(VERSION 3.23)

add_library(json_work)

target_sources(json_work 
PRIVATE
  json_work.cpp
PUBLIC
  FILE_SET HEADERS
  FILES
    json_work.hpp
)

target_link_libraries(json_work
PUBLIC
  structures
PRIVATE
    nlohmann_json::nlohmann_json
)