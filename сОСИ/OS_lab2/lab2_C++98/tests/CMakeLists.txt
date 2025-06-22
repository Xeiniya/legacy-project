
cmake_minimum_required(VERSION 3.10)

project(Tests)

add_executable(tests tests.cpp "../src/Threads.h" "../src/Threads.cpp" "../src/Data.h")

target_include_directories(tests PRIVATE ./src) 

add_test(NAME MyTest COMMAND tests)