cmake_minimum_required(VERSION 2.6)

project(adam)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER		    "/usr/bib/gcc")
SET (CMAKE_CXX_FLAGS        "-O3 -g -std=c++1y -fPIC")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")

add_definitions(-DADAM_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_executable(
	adam
	src/adam.cpp
	src/adam.h
  src/lib/adamcfg.h
  src/lib/adamcfg.cpp
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
)


#add_executable(
#  nntrain
#  src/nntrain.cpp
#)


find_library (PTHREAD pthread)
find_library (CURL curl)
find_library (FAAN fann)
find_library (INFLUXDB influxdb)
find_library (QUOTEK quotek)
find_library (PQXX pqxx)


if (INFLUXDB)

  add_library(
       influxdbbe SHARED
       src/lib/backends/backend.h
       src/lib/backends/influxdb.cpp
  )

  target_link_libraries(
    influxdbbe
    quotek
    influxdb
    curl
  )

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

endif()


target_link_libraries(
        adam
        quotek
        pthread
        dl
)


install (TARGETS adam DESTINATION bin)
#install (TARGETS nntrain DESTINATION tools)

install (TARGETS influxdbbe DESTINATION lib)
install (FILES "install/tools/cbackup.sh" DESTINATION tools)
install (FILES "install/tools/cleanall.sh" DESTINATION tools)

install (DIRECTORY "install/compile" DESTINATION .)
install (DIRECTORY "install/etc" DESTINATION .)
install (DIRECTORY "install/strats" DESTINATION .)
install (DIRECTORY "install/doc" DESTINATION .)

#init script
install (FILES "install/etc/init.d/adam" DESTINATION /etc/init.d/adam)

