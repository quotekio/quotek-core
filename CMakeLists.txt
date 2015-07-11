cmake_minimum_required(VERSION 2.6)

project(adam)

SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_C_COMPILER		    "/usr/bib/clang")
SET (CMAKE_CXX_FLAGS                "-g -O3 -std=c++11 -stdlib=libc++ -fPIC")

SET (CMAKE_LINKER_FLAGS    "-rdynamic")

SET (CMAKE_AR      "/usr/bin/llvm-ar")
SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
SET (CMAKE_NM      "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")

add_definitions(-DADAM_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_executable(
	adam
	src/adam.cpp
	src/adam.h
  src/lib/adamcfg.h
  src/lib/adamcfg.cpp
	src/lib/tsengine.h
	src/lib/tsengine.cpp
  src/lib/btengine.cpp
  src/lib/utils.cpp
  src/lib/broker.hpp
  src/lib/strategyhandler.cpp
  src/lib/narrays.h
  src/lib/narrays.cpp
  src/lib/store.h
  src/lib/store.cpp
  src/lib/moneymanager.cpp
  src/lib/igmlogger.cpp
  src/lib/queue_c.cpp
  src/lib/indice.cpp
  src/lib/results.h
  src/lib/genetics.cpp
  src/lib/network.cpp
  src/lib/aep.cpp
  src/lib/quant.cpp
  src/lib/narrays.cpp
  src/lib/records.cpp
)

add_executable(
  csv2influx
  src/lib/http.cpp
  src/lib/records.cpp
  src/lib/narrays.cpp
  src/csv2influx.cpp
  src/lib/utils.cpp

)

#add_executable(
#  nntrain
#  src/nntrain.cpp
#)

add_library(
       influxdbbe SHARED
       src/lib/backends/backend.h
       src/lib/backends/influxdb.cpp
       src/lib/http.cpp

)

find_library (PTHREAD pthread) 
find_library (CURL curl)
find_library (FAAN fann)
find_library (INFLUXDB influxdb)

target_link_libraries(
        adam
        pthread
        dl
)

target_link_libraries(
  csv2influx
  curl
)


target_link_libraries(
  influxdbbe
  influxdb
  curl
)


install (TARGETS adam DESTINATION bin)
install (TARGETS csv2influx DESTINATION tools)
#install (TARGETS nntrain DESTINATION tools)

install (TARGETS influxdbbe DESTINATION lib)

install (FILES "install/clanglibs/libc++.so" DESTINATION lib)
install (FILES "install/clanglibs/librt.so" DESTINATION lib)
install (FILES "install/tools/cbackup.sh" DESTINATION tools)
install (FILES "install/tools/cleanall.sh" DESTINATION tools)

install (DIRECTORY "install/compile" DESTINATION .)
install (DIRECTORY "install/etc" DESTINATION .)
install (DIRECTORY "install/strats" DESTINATION .)
install (DIRECTORY "install/doc" DESTINATION .)

