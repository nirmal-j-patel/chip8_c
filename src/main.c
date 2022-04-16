#include <stdio.h>
#include <stdlib.h>

#include "state.h"

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

    State state;

    initialize_game_state(&state, argv[1]);

    return EXIT_SUCCESS;
}
