cmake_minimum_required(VERSION 2.6)

project(tests)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER		    "/usr/bib/gcc")
SET (CMAKE_CXX_FLAGS        "-g -std=c++1y -fPIC")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")


add_definitions(-DQATE_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_executable(
	test_nloop
	unit/test_nloop.cpp
)

find_library (CURL curl)
find_library (PTHREAD pthread)
find_library (QUOTEK quotek)

target_link_libraries(
  test_nloop
  quotek
  pthread
  curl
)

