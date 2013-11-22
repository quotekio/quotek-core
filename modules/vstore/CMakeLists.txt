cmake_minimum_required(VERSION 2.6)

project(alert)

SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS                "-std=c++11 -stdlib=libc++ -fPIC")

SET (CMAKE_LINKER_FLAGS    "-rdynamic")

SET (CMAKE_AR      "/usr/bin/llvm-ar")
SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
SET (CMAKE_NM      "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")

add_definitions(-DADAM_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_library(
       vstore SHARED
       src/vstore.cpp
       ../../src/lib/utils.cpp
       ../../src/lib/narrays.cpp
       ../../src/lib/queue_c.cpp
       src/odbc.cpp
)

add_executable(
        vstore2dump
        src/vstore2dump.cpp
        src/odbc.cpp
        ../../src/lib/utils.cpp
        ../../src/lib/narrays.cpp
)


add_executable(
        dump2vstore
        src/dump2vstore.cpp
        src/odbc.cpp
        ../../src/lib/utils.cpp
        ../../src/lib/narrays.cpp
)


find_library (ODBC odbc) 
find_library (PTHREAD pthread)

target_link_libraries(
  vstore
  odbc
  pthread
)


target_link_libraries(
 vstore2dump
 odbc
)

target_link_libraries(
 dump2vstore
 odbc
)
