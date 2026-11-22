cmake_minimum_required(VERSION 3.23)

add_library(presentation)

target_sources(presentation 
PRIVATE 
presentation.cpp
PUBLIC
  FILE_SET HEADERS
  FILES
    presentation.hpp
)

target_link_libraries(presentation
PRIVATE
structures
)