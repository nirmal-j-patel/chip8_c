#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "state.h"
#include <sys/types.h>

enum RegisterOperation {
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

// 00E0
void clear_screen(State* state);

// 00EE
void return_subroutine(State* state);

// 1nnn
void jump_to_location(State* state, u_int16_t address);

// 2nnn, Bnnn
void call_subroutine(State* state, u_int16_t address);

// 3xkk, 4xkk, 5xy0, 9xy0, Ex9E, ExA1
void skip_next_instruction(State* state);

// 6xkk
void set_register_number_value(State* state, u_int8_t register_number, u_int8_t value);

// 7xkk
void add_register_number_value(State* state, u_int8_t register_number, u_int8_t value);

// 8xy0, 8xy1, 8xy2, 8xy3, 8xy4, 8xy5, 8xy6, 8xy7, 8xyE
void manipulate_register_number_using_another_register_number(State* state, u_int8_t vx, u_int8_t vy, enum RegisterOperation operation);

// Annn
void set_register_I_to_value(State* state, u_int16_t value);

// Cxkk
void bitwise_and_value_with_random_byte(State* state, u_int8_t register_number, u_int8_t value);

// Dxyn
void draw(State* state, u_int8_t vx, u_int8_t vy, u_int8_t sprite_length);

// Fx07
void get_delay_timer_to_register_number(State* state, u_int8_t register_number);

// Fx0A
void assign_keypress_value_to_register_number(State* state, u_int8_t register_number);

// Fx15
void set_delay_timer_from_register_number(State* state, u_int8_t register_number);

// Fx18
void set_sound_timer_from_register_number(State* state, u_int8_t register_number);

// Fx1E
void add_register_number_to_I(State* state, u_int8_t register_number);

// Fx29
void set_I_to_register_number_digit_location(State* state, u_int8_t register_number);

// Fx33
void store_BCD_representation_at_I(State* state, u_int8_t register_number);

// Fx55
void store_registers_at_I(State* state, u_int8_t register_number);

// Fx65
void load_registers_from_I(State* state, u_int8_t register_number);

#endif
