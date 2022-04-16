#ifndef __CPU_H__
#define __CPU_H__

#include "state.h"

u_int16_t fetch_next_instruction(State* state);

void parse_instruction_into_nibbles(u_int16_t instruction, u_int8_t* n1, u_int8_t* n2, u_int8_t* n3, u_int8_t* n4);

void parse_instruction_into_bytes(u_int16_t instruction, u_int8_t* b1, u_int8_t* b2);

void execute_next_instruction(State* state);

#endif
