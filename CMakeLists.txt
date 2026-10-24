cmake_minimum_required(VERSION 3.16)

project(RestaurantReservationSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(restaurant
    src/main.cpp
    "include/Customer.hpp"
    "include/Time.hpp"
    "include/Date.hpp"
    "include/pch.hpp"
    "src/Time.cpp"
    "src/Date.cpp")


target_include_directories(restaurant PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_precompile_headers(restaurant PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.hpp"
)