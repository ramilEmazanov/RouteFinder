cmake_minimum_required(VERSION 3.12)

project(
    labwork7
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(route_finder src/main.cpp)

include(CTest)
if(BUILD_TESTING)
    add_test(NAME date_validation
        COMMAND ${CMAKE_COMMAND}
            -DPROGRAM=$<TARGET_FILE:route_finder>
            -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/date_validation.cmake)
endif()

include(tests/coverage.cmake)
