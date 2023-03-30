cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)
include(/mnt/d/LocalRepo/ImbueLight/imbuelight-stair-controller/firmware/pico-sdk/tools/CMakeLists.txt)

project(imbuelight_stair_controller C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD "pico_w")

pico_sdk_init()

add_executable(${PROJECT_NAME}
    src/main.c  
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_pio
    hardware_pwm
    pico_cyw43_arch_none
)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

pico_add_extra_outputs(${PROJECT_NAME})
pico_generate_pio_header(${PROJECT_NAME} 
    ${CMAKE_CURRENT_LIST_DIR}/src/pwm.pio)





