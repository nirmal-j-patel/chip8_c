#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include <SDL.h>

#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "state.h"

void initialize_game_state(State* state, char* filepath)
{
    memset(state->memory, 0, 4096);
    memset(state->display, 0, 64 * 32);
    memset(state->stack, 0, 16);
    memset(state->registers, 0, 16);

    load_rom(state->memory, filepath);

    state->PC = 512;
    state->I = 0;
    state->SP = 0;
    state->DT = 0;
    state->ST = 0;

    state->scale = 10;
    initialize_sdl(state);

    state->AccumulatedTime = 0;
}

void initialize_sdl(State *state) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        exit(1);
    }

    state->window = SDL_CreateWindow("CHIP-8 Emulator",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       64*state->scale, 32*state->scale, 0);

    state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED);
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

    fread(memory + 512, file_size, 1, file);

    fclose(file);
}

void destroy_state(State *state)
{
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
}

void repaint_screen(State *state)
{
    SDL_RenderClear(state->renderer);

    SDL_Rect rect;

    for (int row = 0; row < 32; row++) {
        for (int col = 0; col < 64; col++) {
            rect.x = col*state->scale;
            rect.y = row*state->scale;
            rect.w = state->scale;
            rect.h = state->scale;

            if (state->display[row * 64 + col]) {
                SDL_SetRenderDrawColor( state->renderer, 255, 255, 255, 255 );
            } else {
                SDL_SetRenderDrawColor( state->renderer, 0, 0, 0, 255 );
            }
            SDL_RenderFillRect(state->renderer, &rect);
        }
    }

    SDL_RenderPresent(state->renderer);
}
