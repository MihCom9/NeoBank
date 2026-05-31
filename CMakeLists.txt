cmake_minimum_required(VERSION 3.10)

project(NeoBankPro)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(NeoBank app/main.cpp ${SOURCES})

# Component tests
file(GLOB TEST_SOURCES tests/*.cpp)
add_executable(NeoBank_Tests ${TEST_SOURCES} ${SOURCES})
target_include_directories(NeoBank_Tests PRIVATE include tests)