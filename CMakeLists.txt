cmake_minimum_required(VERSION 3.1)
project(Rfm69Avr)

set(SRC
    src/Spi.cpp
    src/Rfm69.cpp)

add_library(rfm69 ${SRC})

target_include_directories(rfm69
                           PUBLIC  include/
                           PRIVATE include/Rfm69/)

set(PUBLIC_FEATURES
    cxx_binary_literals
    cxx_constexpr
    cxx_static_assert
    cxx_strong_enums
    cxx_variable_templates
    cxx_variadic_macros
)

set(PRIVATE_FEATURES
    cxx_auto_type
    cxx_digit_separators
)

target_compile_features(rfm69
                        PUBLIC  ${PUBLIC_FEATURES}
                        PRIVATE ${PRIVATE_FEATURES})
