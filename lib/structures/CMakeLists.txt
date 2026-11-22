cmake_minimum_required(VERSION 3.23)

add_library(structures)

target_sources(structures 
PRIVATE 
  structs.cpp
PUBLIC
  FILE_SET HEADERS
  FILES
    structs.hpp
)

target_compile_definitions(structures PUBLIC ROOT="${CMAKE_SOURCE_DIR}")
