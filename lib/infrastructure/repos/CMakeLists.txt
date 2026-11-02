cmake_minimum_required(VERSION 3.23)

add_library(repos)

target_sources(repos 
PRIVATE 
repos.cpp
PUBLIC
  FILE_SET HEADERS
  FILES
    repos.hpp
)

target_link_libraries(repos
PUBLIC
structures
business_logic
cache_repo
PRIVATE
nlohmann_json::nlohmann_json
)