cmake_minimum_required(VERSION 3.23)

add_library(stations_parser)

target_sources(stations_parser 
PRIVATE 
stations_parser.cpp
PUBLIC
  FILE_SET HEADERS
  FILES
    stations_parser.hpp
)

target_link_libraries(stations_parser
PRIVATE
nlohmann_json
cpr
business_logic
)