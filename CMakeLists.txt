cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
option(ENABLE_BUILD_AVALON_TEST "build test" OFF)

project(avalon)

add_library(avalon "source/role.cpp")
target_include_directories(
  avalon
  PUBLIC "public"
  PRIVATE "include")

if(ENABLE_BUILD_AVALON_TEST)
  add_subdirectory("test")
endif()
