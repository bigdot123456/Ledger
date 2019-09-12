cmake_minimum_required(VERSION 3.14)
project(Ledger C)

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(cmake/gtest)

add_executable(Ledger main.c)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB_RECURSE TESTS_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
        )

set(BUILD_TESTS OFF CACHE BOOL "Enables tests")

target_include_directories(zxlib_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        )

enable_testing()

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(cmake/gtest)

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")
string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")
#string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=undefined")
#string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fsanitize=undefined")

##############################################################
##############################################################
#  static libs
file(GLOB_RECURSE JSMN_SRC
        #        ${CMAKE_CURRENT_SOURCE_DIR}/src/ledger/deps/jsmn/src/jsmn.c
        )

file(GLOB_RECURSE LIB_SRC
        tests/keccak-tiny.c
        src/ledger/src/lib/crypto.c
        src/ledger/src/lib/utils.c
        src/ledger/src/lib/base58.c
        src/ledger/src/lib/rlp.c
        src/ledger/src/lib/mantx.c
        src/ledger/src/lib/uint256.c
        #        src/ledger/src/lib/json_parser.c
        #        src/ledger/src/lib/tx_parser.c
        #        src/ledger/src/lib/tx_display.c
        #        src/ledger/src/lib/tx_validate.c
        )
file(GLOB_RECURSE TESTS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

add_library(app_lib STATIC ${LIB_SRC} ${JSMN_SRC})
target_include_directories(app_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ledger/deps/ledger-zxlib/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ledger/lib
        #        ${CMAKE_CURRENT_SOURCE_DIR}/src/ledger/deps/jsmn/src
        )

add_executable(unittests ${TESTS_SRC})
target_include_directories(unittests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ledger/src
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        )
target_link_libraries(unittests gtest_main app_lib)
target_link_libraries(unittests gtest_main)

add_test(gtest ${PROJECT_BINARY_DIR}/unittests)

###############################################################
# Force tests to depend from app compiling
###############################################################

#set(DISABLE_DOCKER_BUILDS OFF CACHE BOOL "Disables Docker Builds")
#
#if (NOT DISABLE_DOCKER_BUILDS)
#    add_custom_target(ledger_app
#            COMMAND make build
#            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#            )
#    target_include_directories(ledger_app PUBLIC
#            ${CMAKE_CURRENT_SOURCE_DIR}/src/ledger/src
#            ${CMAKE_CURRENT_SOURCE_DIR}/src/ledger/deps/jsmn/src
#            )
#    add_dependencies(tests ledger_app)
#endif()
