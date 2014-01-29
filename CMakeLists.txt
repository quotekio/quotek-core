cmake_minimum_required(VERSION 2.6)

project(adam)

SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS                "-g -std=c++11 -stdlib=libc++ -fPIC")

SET (CMAKE_LINKER_FLAGS    "-rdynamic")

SET (CMAKE_AR      "/usr/bin/llvm-ar")
SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
SET (CMAKE_NM      "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")

add_definitions(-DADAM_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")


### GTK3 ###
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

### GOOCANVAS ###
include_directories(/usr/include/goocanvas-2.0)

add_executable(
	adam
	src/adam.cpp
	src/adam.h
  src/lib/adamcfg.h
  src/lib/adamcfg.cpp
	src/lib/tsengine.h
	src/lib/tsengine.cpp
  src/lib/utils.h
  src/lib/utils.cpp
  src/lib/brokers/broker.h
  src/lib/strategy.h
  src/lib/strategy.cpp
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
  src/lib/finmath.cpp
  src/lib/narrays.cpp
)

add_executable(
  csv2dump
  src/csv2dump.cpp
  src/lib/utils.cpp

)

#add_executable(
#  nntrain
#  src/nntrain.cpp
#)

add_executable(
  signal
  src/signal/signal.cpp
  src/signal/dump.cpp
  src/signal/graph.cpp
  src/lib/narrays.cpp
  src/lib/utils.cpp
)

add_library(
        igmgw SHARED
        src/lib/brokers/broker.h
        src/lib/brokers/igmgw.cpp
)

add_library(
        igmgw2 SHARED
        src/lib/brokers/broker.h
        src/lib/brokers/igmgw2.cpp
)

find_library (PTHREAD pthread) 
find_library (CURL curl)
find_library (FAAN fann)


target_link_libraries(
        adam
        pthread
        dl
)

target_link_libraries(
  igmgw
  curl
)

target_link_libraries(
  igmgw2
  curl
)


target_link_libraries(
  signal 
  ${GTK3_LIBRARIES}
  goocanvas-2.0
  pthread

)


install (TARGETS adam DESTINATION bin)
install (TARGETS csv2dump DESTINATION tools)
#install (TARGETS nntrain DESTINATION tools)

install (TARGETS igmgw DESTINATION lib)
install (TARGETS igmgw2 DESTINATION lib)


install (FILES "install/clanglibs/libc++.so" DESTINATION lib)
install (FILES "install/clanglibs/librt.so" DESTINATION lib)
install (FILES "install/tools/cbackup.sh" DESTINATION tools)

install (DIRECTORY "install/compile" DESTINATION .)
install (DIRECTORY "install/etc" DESTINATION .)
install (DIRECTORY "install/strats" DESTINATION .)
install (DIRECTORY "install/doc" DESTINATION .)

