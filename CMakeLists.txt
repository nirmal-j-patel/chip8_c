cmake_minimum_required(VERSION 3.5)
project(chip8emu LANGUAGES C)

set(SOURCES
    src/main.c
    src/state.c
    src/cpu.c
    src/instructions.c
)

add_executable(chip8emu ${SOURCES})

target_include_directories(chip8emu
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)



