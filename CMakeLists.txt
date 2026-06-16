cmake_minimum_required(VERSION 3.13)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
project(rp2040-firmware C ASM)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

set(CPU_FLAGS
 -mcpu=cortex-m0plus
    -mthumb        # thumb instructions
   -mfloat-abi=soft
)

set(C_FLAGS
    -nostdlib      # no standard library
    -ffreestanding # no OS
    -Wall          # warnings
    -Os            # optimize for size
)

set(LINKER_FLAGS -T${CMAKE_SOURCE_DIR}/scripts/boot.ld -nostdlib )

set(SOURCES
    boot/boot2.S
    boot/startup.s
    src/uart.c
    src/main.c
)

add_executable(firmware ${SOURCES})

target_compile_options(firmware PRIVATE
    ${CPU_FLAGS}
    ${C_FLAGS}
)

target_link_options(firmware PRIVATE
    ${CPU_FLAGS}
    ${LINKER_FLAGS}
)

target_link_libraries(firmware PRIVATE gcc)
target_include_directories(firmware PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

add_custom_command(TARGET firmware POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary firmware firmware.bin
)