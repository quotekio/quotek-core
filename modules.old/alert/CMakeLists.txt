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
       alert SHARED
       src/alert.cpp
       src/utils.cpp
       src/narrays.cpp
       src/queue_c.cpp
)

find_library (PTHREAD pthread) 
find_library (CURL curl)

target_link_libraries(
  alert
  curl
)

