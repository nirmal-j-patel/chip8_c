#include "instructions.h"
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
    // TODO: remove system function
    system("clear");
}

void jump_to_location(State* state, u_int16_t address)
{
    assert(address <= 4096);
    state->PC = address;
}

void call_subroutine(State* state, u_int16_t address)
{
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
}

void draw(State* state, u_int8_t vx, u_int8_t vy, u_int8_t sprite_length)
{
    // TODO: remove system function
    system("clear");

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

    for (int row = 0; row < 32; row++) {
        for (int col = 0; col < 64; col++) {
            if (state->display[row * 64 + col]) {
                printf("■");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void get_delay_timer_to_register_number(State* state, u_int8_t register_number)
{
}

void assign_keypress_value_to_register_number(State* state, u_int8_t register_number)
{
}

void set_delay_timer_from_register_number(State* state, u_int8_t register_number)
{
}

void set_sound_timer_from_register_number(State* state, u_int8_t register_number)
{
}

void add_register_number_to_I(State* state, u_int8_t register_number)
{
}

void set_I_to_register_number_digit_location(State* state, u_int8_t register_number)
{
}
