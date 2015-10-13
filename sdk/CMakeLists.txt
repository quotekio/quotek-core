cmake_minimum_required(VERSION 2.6)

project(quotek)

SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET (CMAKE_C_COMPILER "/usr/bib/clang")
SET (CMAKE_CXX_FLAGS "-O3 -g -std=c++11 -stdlib=libc++ -fPIC")

add_library(
       quotek STATIC
       src/alchemy.cpp
       src/blackscholes.cpp
       src/broker.cpp
       src/history.cpp
       src/http.cpp
       src/generators.cpp
       src/option.cpp
       src/position.cpp
       src/quant.cpp
       src/record.cpp
       src/techanalysis.cpp
       src/timeutils.cpp
       src/utils.cpp
       
       src/datasources/csv.cpp
       src/datasources/yahoofinance.cpp
       src/datasources/rss.cpp
       src/datasources/twitter.cpp
       src/datasources/quandl.cpp

       src/twitcurl/base64.cpp
       src/twitcurl/HMAC_SHA1.cpp
       src/twitcurl/oauthlib.cpp
       src/twitcurl/SHA1.cpp
       src/twitcurl/twitcurl.cpp
       src/twitcurl/urlencode.cpp
)

find_library (PTHREAD pthread) 
find_library (CURL curl) 

target_link_libraries(
        quotek
        pthread
        curl)

install (TARGETS quotek DESTINATION lib)

install (FILES "src/alchemy.hpp" DESTINATION include/quotek)
install (FILES "src/any.hpp" DESTINATION include/quotek)
install (FILES "src/blackscholes.hpp" DESTINATION include/quotek)
install (FILES "src/broker.hpp" DESTINATION include/quotek)
install (FILES "src/core.hpp" DESTINATION include/quotek)
install (FILES "src/constants.hpp" DESTINATION include/quotek)
install (FILES "src/cqueue.hpp" DESTINATION include/quotek)
install (FILES "src/cvector.hpp" DESTINATION include/quotek)
install (FILES "src/datasource.hpp" DESTINATION include/quotek)
install (FILES "src/generators.hpp" DESTINATION include/quotek)
install (FILES "src/history.hpp" DESTINATION include/quotek)
install (FILES "src/http.hpp" DESTINATION include/quotek)
install (FILES "src/mlstructures.hpp" DESTINATION include/quotek)
install (FILES "src/news.hpp" DESTINATION include/quotek)
install (FILES "src/option.hpp" DESTINATION include/quotek)
install (FILES "src/position.hpp" DESTINATION include/quotek)
install (FILES "src/quant.hpp" DESTINATION include/quotek)
install (FILES "src/record.hpp" DESTINATION include/quotek)
install (FILES "src/strategy.hpp" DESTINATION include/quotek)
install (FILES "src/techanalysis.hpp" DESTINATION include/quotek)
install (FILES "src/timeutils.hpp" DESTINATION include/quotek)
install (FILES "src/utils.hpp" DESTINATION include/quotek)

install (FILES "src/datasources/csv.hpp" DESTINATION include/quotek/datasources)
install (FILES "src/datasources/yahoofinance.hpp" DESTINATION include/quotek/datasources)
install (FILES "src/datasources/rss.hpp" DESTINATION include/quotek/datasources)
install (FILES "src/datasources/twitter.hpp" DESTINATION include/quotek/datasources)
install (FILES "src/datasources/quandl.hpp" DESTINATION include/quotek/datasources)

install (FILES "src/quotek.hpp" DESTINATION include/quotek)
