##############################################################
# cmake head, not need change
cmake_minimum_required(VERSION 3.14)
project(Ledger VERSION 0.0.0)
enable_testing()

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(cmake/gtest)

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")
string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")
#string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=undefined")
#string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fsanitize=undefined")

#### standard header
#############################################################
##############################################################
### include directory

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)
##############################################################
##############################################################
###  static libs
file(GLOB_RECURSE LIB_SRC
        tests/keccak-tiny.c
        lib/crypto.c
        lib/utils.c
        lib/base58.c
        lib/rlp.c
        lib/mantx.c
        lib/uint256.c
        #        lib/json_parser.c
        #        lib/tx_parser.c
        #        lib/tx_display.c
        #        lib/tx_validate.c
        )

add_library(app_lib STATIC ${LIB_SRC})

target_include_directories(app_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        )

#########
### Test directory

file(GLOB_RECURSE TESTS_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
        )

add_executable(unittests ${TESTS_SRC})

target_include_directories(unittests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/hardware
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        )
target_link_libraries(unittests gtest_main app_lib)
target_link_libraries(unittests gtest_main)

add_test(gtest ${PROJECT_BINARY_DIR}/unittests)

##############################################################
##############################################################
add_executable(Ledger main.c)
##############################################################
##############################################################
# 目录结构:
# ./Demo3
#　　|
#　　+—–main.cc
#　　 |
#　　 +—–math/
#　　　　　 |
#　　　　　 +—–MathFunction.cc
#　　　　　 |
#　　　　　 +—–MathFunction.h
# 这种情况需要编写两个CMakeLists.txt文件,分别在Demo3目录下和math目录下.为了方便起见,可以将math目录下的源文件编译为静态链接库,然后由main.cc调用.
# 首先在根目录的CMakeLists.txt中,需要添加math子目录;需要添加静态链接库.
# ① add_subdirectory(math)
# ②target_link_libraries(Demo MathFunctions)
# 最后在math目录中编写CMakeLists.txt,生成动态链接库.
# add_library(MathFunctions ${DIR_LIB_SRCS})


### build option



