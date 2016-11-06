option(RFM69_BUILD_EXAMPLES "Add examples to the 'all' target" OFF)

add_subdirectory(UartStdio/)
set_target_properties(uart-stdio PROPERTIES EXCLUDE_FROM_ALL ON)

function(add_example name)
    if (NOT RFM69_BUILD_EXAMPLES)
        set(EXE_OPTIONS EXCLUDE_FROM_ALL)
    endif ()

    set(TARGET_NAME rfm69-example-${name})
    add_executable(${TARGET_NAME} ${EXE_OPTIONS} ${ARGN})
    target_link_libraries(${TARGET_NAME} rfm69 uart-stdio)
endfunction()

add_definitions(-DSENDER_ADDRESS=1 -DRECEIVER_ADDRESS=2)

add_example(send Send.cpp)
add_example(receive Receive.cpp)
