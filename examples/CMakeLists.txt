include(AvrUpload)

function(add_example name)
    set(TARGET_NAME rfm69-example-${name})
    add_executable(${TARGET_NAME} ${ARGN})
    target_link_libraries(${TARGET_NAME} rfm69 uart-stdio)
    add_upload_target(${TARGET_NAME})
endfunction()

add_definitions(-DSENDER_ADDRESS=1 -DRECEIVER_ADDRESS=2)

add_example(send Send.cpp)
add_example(receive Receive.cpp)
