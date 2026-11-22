add_library(business_logic)

target_sources(business_logic 
PRIVATE
  business_logic.cpp
PUBLIC
  FILE_SET HEADERS
  FILES
    business_logic.hpp
)

target_link_libraries(business_logic
PUBLIC
  structures
)