#include <assert.h>
#include <string.h>

#include "state.h"

void initialize_game_state(State* state, char* filepath)
{
    memset(state->memory, 0, 4096);
    memset(state->display, 0, 64 * 32);
    memset(state->stack, 0, 16);
    memset(state->registers, 0, 16);

    load_rom(state->memory, filepath);

    state->PC = 513;
    state->I = 0;
    state->SP = 0;
    state->DT = 0;
    state->ST = 0;

    state->AccumulatedTime = 0;
}

void load_rom(u_int8_t* memory, char* filepath)
{
    int max_allowed_rom_size = 4096 - 512;
    FILE* file = fopen(filepath, "rb");
    int file_size = 0;

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    assert(file_size <= max_allowed_rom_size);

    fread(memory + 513, file_size, 1, file);

    fclose(file);
}
