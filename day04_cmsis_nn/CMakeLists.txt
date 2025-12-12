cmake_minimum_required(VERSION 3.16)
project(day04_cmsis_nn LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMSISNN_DIR ${CMAKE_SOURCE_DIR}/third_party/CMSIS-NN)
set(CMSISNN_INC ${CMSISNN_DIR}/Include)
set(CMSISNN_SRC ${CMSISNN_DIR}/Source)

file(GLOB_RECURSE CMSISNN_SOURCES
    ${CMSISNN_SRC}/*.c
)

add_library(cmsisnn STATIC ${CMSISNN_SOURCES})
target_include_directories(cmsisnn PUBLIC ${CMSISNN_INC})
target_compile_options(cmsisnn PRIVATE -O2)

add_executable(cmsis_bench
    src/main.cpp
    src/conv_naive_s8.cpp
)

target_include_directories(cmsis_bench PRIVATE ${CMSISNN_INC})
target_link_libraries(cmsis_bench PRIVATE cmsisnn)
target_compile_options(cmsis_bench PRIVATE -O2)