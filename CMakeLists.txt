cmake_minimum_required(VERSION 3.10)
project(rest_api_example)

set(CMAKE_CXX_STANDARD 11)

find_package(cpprestsdk REQUIRED)

add_executable(rest_api_example main.cpp)

target_link_libraries(rest_api_example PRIVATE cpprestsdk::cpprest)