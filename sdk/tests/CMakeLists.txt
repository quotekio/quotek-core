cmake_minimum_required(VERSION 2.6)

project(quotek)

SET (CMAKE_CXX_COMPILER "/usr/bin/g++")
SET (CMAKE_C_COMPILER "/usr/bin/gcc")
SET (CMAKE_CXX_FLAGS "-O3 -std=c++1y -fPIC")

SET (CMAKE_LINKER_FLAGS    "-rdynamic")

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
    test_twitter
    src/test_twitter.cpp
)
   
add_executable(
    test_csv
    src/test_csv.cpp
)


add_executable(
    test_rss
    src/test_rss.cpp
)

add_executable(
    test_etoro
    src/test_etoro.cpp
)


add_executable(
    test_quandl
    src/test_quandl.cpp
)

add_executable(
    test_alchemy
    src/test_alchemy.cpp
)

add_executable(
    test_techanalysis
    src/test_techanalysis.cpp
)

add_executable(
       test_marray
       src/test_marray.cpp
)

add_executable(
       test_json
       src/test_json.cpp
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
        test_csv
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
        test_etoro
        quotek
        pthread
        curl
)

target_link_libraries(
        test_quandl
        quotek
        pthread
        curl
)

target_link_libraries(
        test_twitter
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

target_link_libraries(
        test_techanalysis
        quotek
        pthread
        curl
)

target_link_libraries(
        test_json
        quotek
        pthread
        curl
)