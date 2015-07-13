cmake_minimum_required(VERSION 2.6)

project(quotek)

SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_C_COMPILER		    "/usr/bib/clang")
SET (CMAKE_CXX_FLAGS                "-g -O3 -std=c++11 -stdlib=libc++ -fPIC")

SET (CMAKE_LINKER_FLAGS    "-rdynamic")

SET (CMAKE_AR      "/usr/bin/llvm-ar")
SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
SET (CMAKE_NM      "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")

add_library(
       quotek STATIC
       src/strategy.cpp
)

find_library (PTHREAD pthread) 

target_link_libraries(
        quotek
        pthread
)

install (TARGETS quotek /usr/lib/ )

install (FILES "src/" DESTINATION lib)
install (FILES "install/clanglibs/librt.so" DESTINATION lib)
install (FILES "install/tools/cbackup.sh" DESTINATION tools)
install (FILES "install/tools/cleanall.sh" DESTINATION tools)

install (DIRECTORY "install/compile" DESTINATION .)
install (DIRECTORY "install/etc" DESTINATION .)
install (DIRECTORY "install/strats" DESTINATION .)
install (DIRECTORY "install/doc" DESTINATION .)

