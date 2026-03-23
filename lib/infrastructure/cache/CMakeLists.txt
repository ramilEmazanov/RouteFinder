add_library(cache_repo)

target_sources(cache_repo 
PRIVATE
  cache.cpp
PUBLIC
  FILE_SET HEADERS
  FILES
    cache.hpp
)

target_link_libraries(cache_repo
PUBLIC
  structures
  json_work
  cpr
PRIVATE
    nlohmann_json::nlohmann_json
)