# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(lib-display-ssd1306 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(lib-display-ssd1306 lib-display-ssd1306.c ssd1306/ssd1306.c demo.c )

pico_set_program_name(lib-display-ssd1306 "lib-display-ssd1306")
pico_set_program_version(lib-display-ssd1306 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(lib-display-ssd1306 0)
pico_enable_stdio_usb(lib-display-ssd1306 1)

# Add the standard library to the build
target_link_libraries(lib-display-ssd1306
        pico_stdlib
        hardware_i2c
        hardware_spi
        hardware_uart)

# Add the standard include files to the build
target_include_directories(lib-display-ssd1306 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(lib-display-ssd1306 
        hardware_i2c
        
        )

pico_add_extra_outputs(lib-display-ssd1306)

