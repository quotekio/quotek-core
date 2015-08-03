cmake_minimum_required(VERSION 2.6)

project(quotek)

SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET (CMAKE_C_COMPILER "/usr/bib/clang")
SET (CMAKE_CXX_FLAGS "-O3 -std=c++11 -stdlib=libc++ -fPIC")

SET (CMAKE_LINKER_FLAGS    "-rdynamic")

SET (CMAKE_AR      "/usr/bin/llvm-ar")
SET (CMAKE_LINKER  "/usr/bin/ld")
SET (CMAKE_NM      "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
SET (CMAKE_INSTALL_PREFIX  "/usr")

add_executable(
       test_quant
       src/test_quant.cpp
)

add_executable(
       test_record
       src/test_record.cpp
)

add_executable(
		test_timeutils
		src/test_timeutils.cpp
)

add_executable(
    test_broker
    src/test_broker.cpp
)

add_executable(
    test_strategy
    src/test_strategy.cpp
)

add_executable(
    test_position
    src/test_position.cpp
)

add_executable(
    test_generators
    src/test_generators.cpp
)

add_executable(
    test_blackscholes
    src/test_blackscholes.cpp
)

add_executable(
    test_yahoofinance
    src/test_yahoofinance.cpp
)

add_executable(
    test_rss
    src/test_rss.cpp
)

add_executable(
    test_alchemy
    src/test_alchemy.cpp
)



find_library (quotek QUOTEK) 
find_library (curl CURL) 


target_link_libraries(
        test_quant
        quotek
        pthread
)

target_link_libraries(
        test_record
        quotek
        pthread
)

target_link_libraries(
        test_timeutils
        quotek
        pthread
)

target_link_libraries(
        test_broker
        quotek
        pthread
)

target_link_libraries(
        test_strategy
        quotek
        pthread
)

target_link_libraries(
        test_position
        quotek
        pthread
)

target_link_libraries(
        test_generators
        quotek
        pthread
)

target_link_libraries(
        test_blackscholes
        quotek
        pthread
)

target_link_libraries(
        test_yahoofinance
        quotek
        pthread
        curl
)

target_link_libraries(
        test_rss
        quotek
        pthread
        curl
)

target_link_libraries(
        test_alchemy
        quotek
        pthread
        curl
)