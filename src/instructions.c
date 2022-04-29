#include "instructions.h"
#include "SDL_keycode.h"
#include "state.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

void clear_screen(State* state)
{
    memset(state->display, 0, 64 * 32);
    repaint_screen(state);
}

void jump_to_location(State* state, u_int16_t address)
{
    assert(address <= 4096);
    state->PC = address;
}

void call_subroutine(State* state, u_int16_t address)
{
    state->stack[state->SP] = state->PC;
    state->SP++;
    state->PC = address;
}

void return_subroutine(State *state)
{
    state->SP--;
    state->PC = state->stack[state->SP];
}

void skip_next_instruction(State* state)
{
    state->PC += 2;
    assert(state->PC <= 4096);
}

void set_register_number_value(State* state, u_int8_t register_number, u_int8_t value)
{
    assert(register_number <= 16);
    state->registers[register_number] = value;
}

void add_register_number_value(State* state, u_int8_t register_number, u_int8_t value)
{
    assert(register_number <= 16);
    state->registers[register_number] += value;
}

void manipulate_register_number_using_another_register_number(State* state, u_int8_t vx, u_int8_t vy, enum RegisterOperation operation)
{
    int result = 0;
    const int vx_value = state->registers[vx];
    const int vy_value = state->registers[vy];

    switch (operation) {
    case ASSIGN:
        result = vy_value;
        break;

    case OR:
        result = vx_value | vy_value;
        break;

    case AND:
        result = vx_value & vy_value;
        break;

    case XOR:
        result = vx_value ^ vy_value;
        break;

    case ADD:
        // TODO: set VF = carry
        result = vx_value + vy_value;
        break;

    case SUB:
        // TODO: set VF = NOT borrow
        result = vx_value - vy_value;
        break;

    case SHR:
        result = vx_value >> 1;
        break;

    case SUBN:
        // TODO: set VF = NOT borrow
        result = vy_value - vx_value;
        break;

    case SHL:
        result = vx_value << 1;
        break;

    default:
        printf("Invalid operation");
    }

    state->registers[vx] = result;
}

void set_register_I_to_value(State* state, u_int16_t value)
{
    state->I = value;
}

void bitwise_and_value_with_random_byte(State* state, u_int8_t register_number, u_int8_t value)
{
    const u_int8_t random_byte = rand() % 256;

    state->registers[register_number] = random_byte & value;
}

void draw(State* state, u_int8_t vx, u_int8_t vy, u_int8_t sprite_length)
{
    const u_int8_t vx_val = state->registers[vx] % 64;
    const u_int8_t vy_val = state->registers[vy] % 32;

    // u_int32_t current_pixel = vy_val * 64 + vx_val;
    for (int i = 0; i < sprite_length; i++) {
        u_int32_t current_pixel = (vy_val + i) * 64 + vx_val;
        u_int8_t byte = state->memory[state->I + i];
        u_int8_t bits[8];

        for (int j = 0; j < 8; j++) {
            bits[j] = (byte >> (8 - j - 1)) & 0x1;
        }

        const u_int8_t pixels_to_draw = min(8, 63 - vx_val);

        for (int j = 0; j < pixels_to_draw; j++) {
            state->display[current_pixel++] ^= bits[j];
        }
    }

    repaint_screen(state);
}

void get_delay_timer_to_register_number(State* state, u_int8_t register_number)
{
    state->registers[register_number] = state->DT;
}

void assign_keypress_value_to_register_number(State* state, u_int8_t register_number)
{
    const int key_mapping[16] = {
        SDLK_x,
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_q,
        SDLK_w,
        SDLK_e,
        SDLK_a,
        SDLK_s,
        SDLK_d,
        SDLK_z,
        SDLK_c,
        SDLK_4,
        SDLK_r,
        SDLK_f,
        SDLK_v,
    };

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                SDL_Quit();
                exit(0);
            }

            if (event.type == SDL_KEYDOWN)
            {
                int key = event.key.keysym.sym;

                for (int i = 0; i < 16; i++)
                {
                    if (key_mapping[i] == key)
                    {
                        state->registers[register_number] = i;
                        return;
                    }
                }
            }
        }
    }
}

void set_delay_timer_from_register_number(State* state, u_int8_t register_number)
{
    state->DT = state->registers[register_number];
}

void set_sound_timer_from_register_number(State* state, u_int8_t register_number)
{
    state->ST = state->registers[register_number];
}

void add_register_number_to_I(State* state, u_int8_t register_number)
{
    state->I += state->registers[register_number];
}

void set_I_to_register_number_digit_location(State* state, u_int8_t register_number)
{
    const u_int8_t register_value = state->registers[register_number];
    state->I = register_value * 5;
}

void store_BCD_representation_at_I(State *state, u_int8_t register_number)
{
    u_int8_t vx_value = state->registers[register_number];

    for (int i = 2; i >= 0; i--)
    {
        state->memory[state->I + i] = (vx_value % 10);
        vx_value /= 10;
    }
}

void store_registers_at_I(State *state, u_int8_t register_number)
{
    for (int i = 0; i <= register_number; i++)
    {
        state->memory[state->I + i] = state->registers[i];
    }
}

void load_registers_from_I(State *state, u_int8_t register_number)
{
    for (int i = 0; i <= register_number; i++)
    {
        state->registers[i] = state->memory[state->I + i];
    }
}
