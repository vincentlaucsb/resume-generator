cmake_minimum_required(VERSION 3.8)
project("resume-generator")
set(CMAKE_CXX_STANDARD 17)
set(RESUME_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory("include")

enable_testing()
add_subdirectory("tests")