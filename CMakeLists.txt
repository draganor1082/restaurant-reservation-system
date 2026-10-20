cmake_minimum_required(VERSION 3.16)

project(RestaurantReservationSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(restaurant
    src/main.cpp
    "include/Customer.hpp"
    "include/Time.hpp"
    "src/Time.cpp")

target_include_directories(restaurant PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)