cmake_minimum_required (VERSION 2.8)
project (main)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

# c++11 support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

set(LIB_LIST)

# libuv
find_path(LIBUV_INCLUDE_DIRS NAMES uv.h)
find_library(LIBUV_LIBRARIES NAMES uv)
if (LIBUV_INCLUDE_DIRS AND LIBUV_LIBRARIES)
    include_directories("${LIBUV_INCLUDE_DIRS}")
    link_directories("${LIBUV_LIBRARIES}")
    list(APPEND LIB_LIST uv)
endif()

# application
INCLUDE_DIRECTORIES(include)
file(GLOB_RECURSE DIR_SRCS "src/*.cpp")
file(GLOB_RECURSE DIR_MAINS "main/*.cpp")

add_executable(main ${DIR_SRCS} ${DIR_MAINS})
TARGET_LINK_LIBRARIES(main ${LIB_LIST})

# library
ADD_LIBRARY(websocket STATIC ${DIR_SRCS})
TARGET_LINK_LIBRARIES(websocket ${LIB_LIST})

# cpplint
file(GLOB_RECURSE ALL_CHECK_FILES "include/*.h" "src/*.cpp" "main/*.cpp")

add_custom_target(
        cpplint ALL
        COMMAND cpplint
        ${ALL_CHECK_FILES}
)

# cppcheck
add_custom_target(
        cppcheck ALL
        COMMAND cppcheck
        --enable=all
        --std=c++11
        --verbose
        --quiet
        ${ALL_CHECK_FILES}
)

# valgrind
find_program(VALGRIND "valgrind")
if(VALGRIND)
    add_custom_target(valgrind
        COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes $<TARGET_FILE:main>)
endif()

# test
add_subdirectory(tests)
enable_testing()
add_test(unittest ${PROJECT_SOURCE_DIR}/bin/unittest)

# coverage
OPTION (ENABLE_GCOV "Enable gcov (debug, Linux builds only)" ON)
IF(ENABLE_GCOV)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    list(APPEND LIB_LIST gcov)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/deps/cmake)
    include(coverage)
    set(TESTS unittest)
    setup_target_for_coverage(coverage ${TESTS} coverage)
ENDIF()
