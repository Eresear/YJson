cmake_minimum_required (VERSION 2.6)
project (yjson_test C)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Wall")
endif()

add_library(yjson yjson.c)
add_executable(yjson_test test.c)
target_link_libraries(yjson_test yjson)