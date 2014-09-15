cmake_minimum_required(VERSION 2.6)

project(tests)

SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS                "-std=c++11 -stdlib=libc++ -fPIC")

SET (CMAKE_LINKER_FLAGS    "-rdynamic")

SET (CMAKE_AR      "/usr/bin/llvm-ar")
SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
SET (CMAKE_NM      "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")

add_definitions(-DADAM_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_executable(
	test_records
	unit/test_records.cpp
        ../src/lib/records.cpp
        ../src/lib/narrays.cpp
)
