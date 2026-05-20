cmake_minimum_required(VERSION 3.13)

# Language standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set Pico SDK path
set(PICO_SDK_PATH "/home/victor/Pico/pico-sdk")

# VS Code Pico extension support
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)

set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)

if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# Board type
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Import Pico SDK
include(pico_sdk_import.cmake)

# Project name
project(DispenserMain C CXX ASM)

# Initialize Pico SDK
pico_sdk_init()

# Create executable
add_executable(DispenserMain
    DispenserMain.cpp
)

# Program info
pico_set_program_name(DispenserMain "DispenserMain")
pico_set_program_version(DispenserMain "0.1")

# Enable USB serial access
pico_enable_stdio_usb(DispenserMain 1)

# Disable UART serial
pico_enable_stdio_uart(DispenserMain 0)

# Link required libraries
target_link_libraries(DispenserMain
    pico_stdlib
    hardware_adc
    hardware_pwm
    pico_multicore
)

# Include directories
target_include_directories(DispenserMain PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Generate UF2, BIN, ELF files
pico_add_extra_outputs(DispenserMain)
