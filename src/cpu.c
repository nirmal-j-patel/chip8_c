#include <assert.h>

#include "cpu.h"

u_int16_t fetch_next_instruction(State* state)
{
    const u_int8_t byte1 = state->memory[state->PC];
    const u_int8_t byte2 = state->memory[state->PC + 1];
    const u_int16_t instruction = (byte1 << 8) + byte2;

    state->PC += 2;

    assert(state->PC <= 4096);

    return instruction;
}

void parse_instruction_into_nibbles(u_int16_t instruction, u_int8_t* n1, u_int8_t* n2, u_int8_t* n3, u_int8_t* n4)
{
    *n1 = (instruction & 0xF000) >> 12;
    *n2 = (instruction & 0x0F00) >> 8;
    *n3 = (instruction & 0x00F0) >> 4;
    *n4 = instruction & 0x000F;
}

void parse_instruction_into_bytes(u_int16_t instruction, u_int8_t* b1, u_int8_t* b2)
{
    *b1 = (instruction & 0xFF00) >> 8;
    *b2 = instruction & 0x00FF;
}

void execute_next_instruction(State* state)
{
    const u_int16_t next_instruction = fetch_next_instruction(state);

    u_int8_t nibble1, nibble2, nibble3, nibble4;
    parse_instruction_into_nibbles(next_instruction, &nibble1, &nibble2, &nibble3, &nibble4);

    u_int8_t byte1, byte2;
    parse_instruction_into_bytes(next_instruction, &byte1, &byte2);

    switch (nibble1) {
    case 0x0:
        switch (next_instruction) {
        // CLS - clear screen
        case 0x00E0:
            break;

        // RET - return from a subroutine
        case 0x00EE:
            break;

        // SYS addr - ignored for now
        // TODO: consider implementing this
        default:
            break;
        }
        break;

    // JMP addr - jump to address
    case 0x1:
        break;

    // CALL addr - call subroutine at address
    case 0x2:
        break;

    // SE Vx, byte - skip next instruction if Vx == kk
    case 0x3:
        break;

    // SNE Vx, byte - skip next instruction if Vx != kk
    case 0x4:
        break;

    // SE Vx, Vy - skip next instruction if Vx == Vy
    case 0x5:
        break;

    // LD Vx, byte - set Vx = kk
    case 0x6:
        break;

    // Add Vx, byte - set Vx = Vx + kk
    case 0x7:
        break;

    case 0x8:
        switch (nibble4) {
        // LD Vx, Vy - set Vx = Vy
        case 0x0:
            break;

        // OR Vx, Vy - set Vx = Vx OR Vy
        case 0x1:
            break;

        // AND Vx, Vy - set Vx = Vx AND Vy
        case 0x2:
            break;

        // XOR Vx, Vy - set Vx = Vx XOR Vy
        case 0x3:
            break;

        // ADD Vx, Vy - set Vx = Vx + Vy, set VF = carry
        case 0x4:
            break;

        // SUB Vx, Vy - set Vx = Vx - Vy, set VF = NOT borrow
        case 0x5:
            break;

        // SHR Vx {, Vy} - set Vx = Vx SHR 1
        case 0x6:
            break;

        // SUBN Vx, Vy - set Vx = Vy - Vx
        case 0x7:
            break;

        // SHL Vx {, Vy} - set Vx = Vx SHL 1
        case 0xE:
            break;

        default:
            printf("Uknown instruction\n");
            break;
        }
        break;

    // SNE Vx, Vy - skip next instruction if Vx != Vy
    case 0x9:
        break;

    // LD I, addr - set I = nnn
    case 0xA:
        break;

    // JMP V0, addr - jump to location nnn + V0
    case 0xB:
        break;

    // RND vx, byte - set Vx = random byte AND kk
    case 0xC:
        break;

    // DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I
    // at (Vx, Vy), set VF = collision
    case 0xD:
        break;

    case 0xE:
        switch (byte2) {
        // SKP Vx - skip next instruction if key with the value of Vx is pressed
        case 0x9E:
            break;

        // SKNP Vx - skip next instruction if key with the value of Vx is not pressed
        case 0xA1:
            break;
        }
        break;

    case 0xF:
        switch (byte2) {
        //
        case 0x07:
            break;

        //
        case 0x0A:
            break;

        //
        case 0x15:
            break;
        
        //
        case 0x18:
            break;

        //
        case 0x1E:
            break;

        //
        case 0x29:
            break;

        //
        case 0x33:
            break;

        //
        case 0x55:
            break;

        //
        case 0x65:
            break;
        }
        break;

    default:
        printf("Unknown instruction\n");
        break;
    }
}