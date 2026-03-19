cmake_minimum_required(VERSION 3.12)

project(
    labwork7
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

include_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB LIB_SOURCES "lib/**/*.cpp")

add_executable(labwork7 main.cpp ${LIB_SOURCES})

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG c492b4fed1fd7288eacec6fe8d89e37df78d373a)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(labwork7 PRIVATE 
    nlohmann_json::nlohmann_json
    cpr::cpr)
