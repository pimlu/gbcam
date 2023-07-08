cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall)


add_executable(gbcam
        src/main.c
        src/utils.c
        src/drivers/gbcam.c
        src/drivers/sharp.c
        src/drivers/usb/usb_serial.c
        src/drivers/usb/usb_descriptors.c
        src/tasks/debug_led.c
        src/tasks/video.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(gbcam PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/usb/include)

# pull in common dependencies
target_link_libraries(gbcam
        pico_stdlib
        tinyusb_device tinyusb_board
        pico_multicore hardware_adc hardware_pwm hardware_spi
        )

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(gbcam)

# enable usb output, disable uart output
pico_enable_stdio_usb(gbcam 1)
pico_enable_stdio_uart(gbcam 0)
