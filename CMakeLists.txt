cmake_minimum_required(VERSION 3.14)
project(five_color C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "--std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes -g")

add_executable(five_color main.c graph.c graph.h Stack.c Stack.h tools.c tools.h)

target_link_libraries(five_color m)

add_executable(test_stack testStack.c graph.c graph.h Stack.c Stack.h)