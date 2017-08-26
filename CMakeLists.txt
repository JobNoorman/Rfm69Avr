cmake_minimum_required(VERSION 3.1)
project(Rfm69Avr)

add_compile_options(-Wall -std=c++17)

add_subdirectory(dependencies/)

set(SRC
    src/Spi.cpp
    src/Rfm69.cpp)

add_library(rfm69 ${SRC})

target_include_directories(rfm69
                           PUBLIC  include/
                           PRIVATE include/Rfm69/)

option(RFM69_BUILD_EXAMPLES "Build examples" OFF)

if (RFM69_BUILD_EXAMPLES)
    add_subdirectory(examples/)
endif ()
