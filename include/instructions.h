#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "state.h"
#include <sys/types.h>

enum RegisterOperation
{
    ASSIGN,
    OR,
    AND,
    XOR,
    ADD,
    SUB,
    SHR,
    SUBN,
    SHL,
};

void clear_screen(State *state);

void jump_to_location(State *state, u_int16_t address);

void call_subroutine(State *state, u_int16_t address);

void skip_next_instruction(State *state);

void set_register_value(State *state, u_int8_t register, u_int8_t value);

void add_register_value(State *state, u_int8_t register, u_int8_t value);

void manipulate_register_using_another_register(State *state, u_int8_t vx, u_int8_t vy, enum RegisterOperation operation);

void set_register_I(State *state, u_int16_t value);

void bitwise_and_value_with_random_byte(State *state, u_int8_t register, u_int8_t value);

void draw(State *state, u_int8_t vx, u_int8_t vy, u_int8_t sprite_length);

void get_delay_timer_to_register(State *state, u_int8_t register);

void assign_keypress_value_to_register(State *state, u_int8_t register);

void set_delay_timer_from_register(State *state, u_int8_t register);

void set_sound_timer_from_register(State *state, u_int8_t register);

void add_register_to_I(State *state, u_int8_t register);

void set_I_to_register_digit_location(State *state, u_int8_t register);


#endif
