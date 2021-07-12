cmake_minimum_required(VERSION 3.19)
project(SummerPractice2021Cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(case_switcher main.cpp)

target_link_libraries(case_switcher pthread)
#add_compile_options(-O3)

