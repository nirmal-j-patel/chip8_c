#ifndef __STATE_H__
#define __STATE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct State {
    u_int8_t memory[4096];
    u_int8_t display[64 * 32];
    u_int16_t PC;
    u_int16_t I;
    u_int16_t stack[16];
    u_int8_t SP;
    u_int8_t DT;
    u_int8_t ST;
    u_int8_t registers[16];

    u_int64_t AccumulatedTime;
} State;

void initialize_game_state(State* state, char* filepath);

void load_rom(u_int8_t* memory, char* filepath);

#endif
