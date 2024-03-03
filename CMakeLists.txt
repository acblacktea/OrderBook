cmake_minimum_required(VERSION 3.26)
project(OrderBook)

set(CMAKE_CXX_STANDARD 23)

include_directories(src)

add_subdirectory(src)
add_subdirectory(Tests)
add_subdirectory(lib/googletest)