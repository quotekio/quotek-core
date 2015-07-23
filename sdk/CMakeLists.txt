cmake_minimum_required(VERSION 2.6)

project(quotek)

SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET (CMAKE_C_COMPILER "/usr/bib/clang")
SET (CMAKE_CXX_FLAGS "-O3 -std=c++11 -stdlib=libc++ -fPIC")

SET (CMAKE_LINKER_FLAGS    "-rdynamic")

SET (CMAKE_AR      "/usr/bin/llvm-ar")
SET (CMAKE_LINKER  "/usr/bin/ld")
SET (CMAKE_NM      "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
SET (CMAKE_INSTALL_PREFIX  "/usr")

add_library(
       quotek STATIC
       src/broker.cpp
       src/generators.cpp
       src/option.cpp
       src/position.cpp
       src/quant.cpp
       src/record.cpp
       src/strategy.cpp
       src/timeutils.cpp
)

find_library (PTHREAD pthread) 

target_link_libraries(
        quotek
        pthread
)

install (TARGETS quotek DESTINATION lib )

install (FILES "src/any.hpp" DESTINATION include/quotek)
install (FILES "src/broker.hpp" DESTINATION include/quotek)
install (FILES "src/core.hpp" DESTINATION include/quotek)
install (FILES "src/cqueue.hpp" DESTINATION include/quotek)
install (FILES "src/cvector.hpp" DESTINATION include/quotek)
install (FILES "src/generators.hpp" DESTINATION include/quotek)
install (FILES "src/position.hpp" DESTINATION include/quotek)
install (FILES "src/quant.hpp" DESTINATION include/quotek)
install (FILES "src/record.hpp" DESTINATION include/quotek)
install (FILES "src/strategy.hpp" DESTINATION include/quotek)
install (FILES "src/timeutils.hpp" DESTINATION include/quotek)
