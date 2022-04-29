#include <assert.h>
#include <sys/types.h>

#include "SDL_events.h"
#include "SDL_scancode.h"
#include "SDL_timer.h"
#include "cpu.h"
#include "instructions.h"

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

u_int16_t get_last_three_nibbles(u_int16_t instruction)
{
    return instruction & 0x0FFF;
}

int execute_next_instruction(State* state)
{
    const u_int64_t current_time = SDL_GetTicks64();
    state->accumulated_time += current_time - state->last_sync_time;

    const u_int64_t step_time = 1000/60;
    while (state->accumulated_time >= step_time)
    {
        state->accumulated_time -= step_time;
        if (state->DT >0)
        {
            state->DT--;
        }

        if (state->ST > 0)
        {
            state->ST--;
        }
    }

    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
    const int key_mapping[16] = {
        SDL_SCANCODE_X,
        SDL_SCANCODE_1,
        SDL_SCANCODE_2,
        SDL_SCANCODE_3,
        SDL_SCANCODE_Q,
        SDL_SCANCODE_W,
        SDL_SCANCODE_E,
        SDL_SCANCODE_A,
        SDL_SCANCODE_S,
        SDL_SCANCODE_D,
        SDL_SCANCODE_Z,
        SDL_SCANCODE_C,
        SDL_SCANCODE_4,
        SDL_SCANCODE_R,
        SDL_SCANCODE_F,
        SDL_SCANCODE_V,
    };
    for (int i = 0; i < 16; i++)
    {
        state->key_pressed_state[i] = keyboard_state[key_mapping[i]];
    }

    const u_int16_t next_instruction = fetch_next_instruction(state);

    u_int8_t nibble1, nibble2, nibble3, nibble4;
    parse_instruction_into_nibbles(next_instruction, &nibble1, &nibble2, &nibble3, &nibble4);

    u_int8_t byte1, byte2;
    parse_instruction_into_bytes(next_instruction, &byte1, &byte2);

    u_int16_t last_three_nibbles = get_last_three_nibbles(next_instruction);

    switch (nibble1) {
    case 0x0:
        switch (next_instruction) {
        // CLS - clear screen
        case 0x00E0:
            clear_screen(state);
            break;

        // RET - return from a subroutine
        case 0x00EE:
            return_subroutine(state);
            break;

        // SYS addr - ignored for now
        // TODO: consider implementing this
        default:
            break;
        }
        break;

    // JMP addr - jump to address
    case 0x1:
        jump_to_location(state, last_three_nibbles);
        break;

    // CALL addr - call subroutine at address
    case 0x2:
        call_subroutine(state, last_three_nibbles);
        break;

    // SE Vx, byte - skip next instruction if Vx == kk
    case 0x3:
        if (state->registers[nibble2] == byte2) {
            skip_next_instruction(state);
        }
        break;

    // SNE Vx, byte - skip next instruction if Vx != kk
    case 0x4:
        if (state->registers[nibble2] != byte2) {
            skip_next_instruction(state);
        }
        break;

    // SE Vx, Vy - skip next instruction if Vx == Vy
    case 0x5:
        assert(nibble4 == 0);
        if (state->registers[nibble2] == state->registers[nibble3]) {
            skip_next_instruction(state);
        }
        break;

    // LD Vx, byte - set Vx = kk
    case 0x6:
        set_register_number_value(state, nibble2, byte2);
        break;

    // Add Vx, byte - set Vx = Vx + kk
    case 0x7:
        add_register_number_value(state, nibble2, byte2);
        break;

    case 0x8: {
        enum RegisterOperation operation;
        switch (nibble4) {
        // LD Vx, Vy - set Vx = Vy
        case 0x0:
            operation = ASSIGN;
            break;

        // OR Vx, Vy - set Vx = Vx OR Vy
        case 0x1:
            operation = OR;
            break;

        // AND Vx, Vy - set Vx = Vx AND Vy
        case 0x2:
            operation = AND;
            break;

        // XOR Vx, Vy - set Vx = Vx XOR Vy
        case 0x3:
            operation = XOR;
            break;

        // ADD Vx, Vy - set Vx = Vx + Vy, set VF = carry
        case 0x4:
            operation = ADD;
            break;

        // SUB Vx, Vy - set Vx = Vx - Vy, set VF = NOT borrow
        case 0x5:
            operation = SUB;
            break;

        // SHR Vx {, Vy} - set Vx = Vx SHR 1
        case 0x6:
            operation = SHR;
            break;

        // SUBN Vx, Vy - set Vx = Vy - Vx
        case 0x7:
            operation = SUBN;
            break;

        // SHL Vx {, Vy} - set Vx = Vx SHL 1
        case 0xE:
            operation = SHL;
            break;

        default:
            printf("Uknown instruction\n");
            break;
        }
        manipulate_register_number_using_another_register_number(state, nibble2, nibble3, operation);
    } break;

    // SNE Vx, Vy - skip next instruction if Vx != Vy
    case 0x9:
        assert(nibble4 == 0);
        if (state->registers[nibble2] != state->registers[nibble3]) {
            skip_next_instruction(state);
        }
        break;

    // LD I, addr - set I = nnn
    case 0xA:
        set_register_I_to_value(state, last_three_nibbles);
        break;

    // JMP V0, addr - jump to location nnn + V0
    case 0xB:
        jump_to_location(state, state->registers[0] + last_three_nibbles);
        break;

    // RND vx, byte - set Vx = random byte AND kk
    case 0xC:
        bitwise_and_value_with_random_byte(state, nibble2, byte2);
        break;

    // DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I
    // at (Vx, Vy), set VF = collision
    case 0xD:
        draw(state, nibble2, nibble3, nibble4);
        break;

    case 0xE:
        switch (byte2) {
        // SKP Vx - skip next instruction if key with the value of Vx is pressed
        case 0x9E:
            if (state->key_pressed_state[state->registers[nibble2]])
            {
                skip_next_instruction(state);
            }
            break;

        // SKNP Vx - skip next instruction if key with the value of Vx is not pressed
        case 0xA1:
            if (!state->key_pressed_state[state->registers[nibble2]])
            {
                skip_next_instruction(state);
            }
            break;
        }
        break;

    case 0xF:
        switch (byte2) {
        // LD Vx, DT - set Vx = delay timer value
        case 0x07:
            get_delay_timer_to_register_number(state, nibble2);
            break;

        // LD Vx, K - wait for a key press, store the value of the key in Vx
        case 0x0A:
            assign_keypress_value_to_register_number(state, nibble2);
            break;

        // LD DT, Vx - set delay timer = Vx
        case 0x15:
            set_delay_timer_from_register_number(state, nibble2);
            break;

        // LD ST, Vx - set sound timer = Vx
        case 0x18:
            set_sound_timer_from_register_number(state, nibble2);
            break;

        // ADD I, Vx - set I = I + Vx
        case 0x1E:
            add_register_number_to_I(state, nibble2);
            break;

        // LD F, Vx - set I = location of sprite for digit Vx
        case 0x29:
            set_I_to_register_number_digit_location(state, nibble2);
            break;

        // LD B, Vx - store BCD representation of Vx in memory locations I, I+1, and I+2
        case 0x33:
            store_BCD_representation_at_I(state, nibble2);
            break;

        // LD B, Vx - store BCD representation of Vx in memory locations I, I+1, and I+2
        case 0x55:
            store_registers_at_I(state, nibble2);
            break;

        // LD Vx, [I] - read registers V0 through Vx from memory starting at location I
        case 0x65:
            load_registers_from_I(state, nibble2);
            break;
        }
        break;

    default:
        printf("Unknown instruction\n");
        break;
    }

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return SDL_QUIT;
        }
    }

    return 0;
}
