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
    "src/Date.cpp"
)


target_include_directories(restaurant PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_precompile_headers(restaurant PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.hpp"
)

# Enable googletest testing
enable_testing()

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Establish test environment 

add_executable(restaurant_tests
    Tests/ReservationTests.cpp
    src/Reservation.cpp
    src/Date.cpp
    src/Time.cpp
)

target_include_directories(restaurant_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(restaurant_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(restaurant_tests)

target_precompile_headers(restaurant_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.hpp"
)