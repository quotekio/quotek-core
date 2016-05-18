cmake_minimum_required(VERSION 2.6)

project(tests)

SET (CMAKE_CXX_COMPILER             "/usr/bin/g++")
SET (CMAKE_CXX_FLAGS                "-std=c++11 -fPIC")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")

add_definitions(-QATE_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_executable(
	test_utils
	unit/test_utils.cpp
    ../src/lib/utils.cpp
)


add_executable(
	test_records
	unit/test_records.cpp
        ../src/lib/records.cpp
        ../src/lib/narrays.cpp
)

add_executable(
	test_backend_influxdb
	../src/lib/records.cpp
	../src/lib/narrays.cpp
	../src/lib/http.cpp
	../src/lib/utils.cpp
	unit/test_backend_influxdb.cpp
)




find_library (CURL curl)

target_link_libraries(
  test_backend_influxdb
  curl
)

