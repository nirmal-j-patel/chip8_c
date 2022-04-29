#ifndef __STATE_H__
#define __STATE_H__

#include "SDL_render.h"
#include "SDL_video.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <SDL.h>

typedef struct State {
    // CHIP8 architecture specific variables
    u_int8_t memory[4096];
    u_int8_t display[64 * 32];
    u_int16_t PC;
    u_int16_t I;
    u_int16_t stack[16];
    u_int8_t SP;
    u_int8_t DT;
    u_int8_t ST;
    u_int8_t registers[16];

    // SDL specific variables
    int scale;
    SDL_Window *window;
    SDL_Renderer *renderer;
    u_int8_t key_pressed_state[16];

    u_int64_t last_sync_time;
    u_int64_t accumulated_time;
} State;

void initialize_game_state(State* state, char* filepath);

void initialize_sdl(State *state);

void load_rom(u_int8_t* memory, char* filepath);

void destroy_state(State *state);

void repaint_screen(State *state);

#endif
