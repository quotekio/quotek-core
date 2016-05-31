cmake_minimum_required(VERSION 2.6)

project(qate)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER		    "/usr/bib/gcc")
SET (CMAKE_CXX_FLAGS        "-g -std=c++1y -fPIC")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")

add_definitions(-DQATE_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_executable(
	qate
	src/qate.cpp
	src/qate.h
  src/lib/qatecfg.h
  src/lib/qatecfg.cpp
	src/lib/tsengine.h
	src/lib/tsengine.cpp
  src/lib/hsbt.cpp
  src/lib/utils.cpp
  src/lib/broker.hpp
  src/lib/strategyhandler.cpp
  src/lib/narrays.h
  src/lib/narrays.cpp
  src/lib/moneymanager.cpp
  src/lib/igmlogger.cpp
  src/lib/queue_c.cpp
  src/lib/indice.cpp
  src/lib/results.h
  src/lib/genetics.cpp
  src/lib/network.cpp
  src/lib/aep.cpp
  src/lib/narrays.cpp
  src/lib/aep_ws.cpp
  
)


#add_executable(
#  nntrain
#  src/nntrain.cpp
#)


find_library (PTHREAD pthread)
find_library (CURL curl)
find_library (FAAN fann)
find_library (QUOTEK quotek)
find_library (PQXX pqxx)
find_library (HIREDIS hiredis)

if (HIREDIS) 

  add_library(
       rediscache SHARED
       src/lib/caches/cache.h
       src/lib/caches/redis.cpp
  )

  target_link_libraries(
    rediscache
    hiredis
    quotek
    curl
  )

  install (TARGETS rediscache DESTINATION lib)

endif()


if (PQXX)
  add_library(
      postgresqlbe SHARED
      src/lib/backends/backend.h
      src/lib/backends/postgresql.cpp
  )
  target_link_libraries(
    postgresqlbe
    quotek
    pqxx
  )  

  install (TARGETS postgresqlbe DESTINATION lib)

endif()


add_library(
       influxdbbe SHARED
       src/lib/backends/backend.h
       src/lib/backends/influxdb.cpp
)

target_link_libraries(
    influxdbbe
    quotek
    curl
)

add_library(
       influxdbbe13 SHARED
       src/lib/backends/backend.h
       src/lib/backends/influxdb13.cpp
)

target_link_libraries(
    influxdbbe13
    quotek
    curl
)


install (TARGETS influxdbbe DESTINATION lib)

target_link_libraries(
        qate
        quotek
        pthread
        dl
)


install (TARGETS qate DESTINATION bin)
#install (TARGETS nntrain DESTINATION tools)

install (DIRECTORY "install/etc" DESTINATION .)

#init script
install (FILES "install/etc/init.d/qate" DESTINATION /etc/init.d/)

