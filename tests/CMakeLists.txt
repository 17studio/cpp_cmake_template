cmake_minimum_required (VERSION 2.8)
project (unittest)

add_subdirectory(../deps/googletest lib) # generate deps library

set(LIB_LIST)

# gtest
find_path(GTEST_INCLUDE_DIRS NAMES gtest.h  HINTS "../deps/googletest/googletest/include/gtest")
find_library(GTEST_LIBRARIES NAMES gtest_main HINTS "../lib")
if (GTEST_INCLUDE_DIRS)
    include_directories("${GTEST_INCLUDE_DIRS}")
    if(GTEST_LIBRARIES)
        link_directories("${GTEST_LIBRARIES}")
        list(APPEND LIB_LIST gtest)
        list(APPEND LIB_LIST gtest_main)
    endif()
endif()

# application
INCLUDE_DIRECTORIES(include)
file(GLOB_RECURSE DIR_SRCS "../src/*.cpp")
file(GLOB_RECURSE DIR_TESTS "../tests/*.cpp")

add_executable(unittest ${DIR_SRCS} ${DIR_TESTS})
TARGET_LINK_LIBRARIES(unittest ${LIB_LIST})

# coverage
OPTION (ENABLE_GCOV "Enable gcov (debug, Linux builds only)" ON)
IF(ENABLE_GCOV)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    list(APPEND LIB_LIST gcov)
    set(CMAKE_BUILD_TYPE Debug)
ENDIF()