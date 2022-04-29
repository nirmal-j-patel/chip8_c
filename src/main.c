#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "SDL_events.h"
#include "cpu.h"
#include "state.h"

#include <unistd.h>

void print_help(const char* program_name)
{
    printf("USAGE:\n");
    printf("    %s ROM_location\n", program_name);

    exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        print_help(argv[0]);
    }

    srand(time(NULL));

    State state;

    initialize_game_state(&state, argv[1]);

    int close = 0;
    while (close != SDL_QUIT) {
        close = execute_next_instruction(&state);
        usleep(2000);
    }

    destroy_state(&state);

    return EXIT_SUCCESS;
}
