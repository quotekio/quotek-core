cmake_minimum_required(VERSION 2.6)

project(quotek)

SET (CMAKE_CXX_COMPILER "/usr/bin/g++")
SET (CMAKE_C_COMPILER "/usr/bin/gcc")
SET (CMAKE_CXX_FLAGS "-O3 -std=c++1y -fPIC")

SET (CMAKE_LINKER_FLAGS    "-rdynamic")

add_executable(
       bench_record
       src/bench_record.cpp
)


find_library (quotek QUOTEK) 
find_library (curl CURL) 
find_library (benchmark BENCHMARK)

target_link_libraries(
        bench_record
        benchmark
        quotek
        pthread
)

