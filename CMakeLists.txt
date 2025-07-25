cmake_minimum_required(VERSION 3.25..3.31)

project(LT-XPLR LANGUAGES CXX VERSION 0.1.0)

option(LTXPLR_ENABLE_OMP "Enable OpenMP support. Default: OFF. Values: { ON, OFF }." OFF)

if(LTXPLR_ENABLE_OMP)
  find_package(OpenMP REQUIRED)
endif()

add_subdirectory(include/ltxplr)
add_subdirectory(benchmarks)
