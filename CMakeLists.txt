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
    "include/Reservation.hpp"
    "include/Table/Table.hpp"
    "include/Table/RegularTable.hpp"
    "include/Table/VIPTable.hpp"
    "src/Table/Table.cpp"
    "src/Table/VIPTable.cpp"
    "src/Table/RegularTable.cpp"
    "src/Time.cpp"
    "src/Reservation.cpp"
    "src/Date.cpp" "src/Table/Table.cpp" "src/Table/RegularTable.cpp" "src/Table/VIPTable.cpp" "src/Reservation.cpp")


target_include_directories(restaurant PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_precompile_headers(restaurant PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.hpp"
)