#define _DEFAULT_SOURCE

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "world.h"
#include "tile.h"
#include "tile_variation.h"

int main(int argc, char **argv)
{
    int world_width = 100;
    int world_height = 25;
    bool interactive_mode = false;

    int next_option_i = 1;
    char *option = NULL;
    if (argc > next_option_i)
    {
        option = argv[next_option_i];
        if (strcmp(option, "-i") == 0)
        {
            interactive_mode = true;
            next_option_i++;
        }
    }

    if (argc > next_option_i)
    {
        option = argv[next_option_i];
        world_width = atoi(option);
        next_option_i++;
    }
    if (argc > next_option_i)
    {
        option = argv[next_option_i];
        world_height = atoi(option);
        next_option_i++;
    }

    // get variations
    struct all_variations variations = variation_read_config("config.json");

    printf("all variations (Tile and weights): \n");
    for (unsigned int i = 0; i < variations.num_variations; i++)
    {
        struct variation *var = &variations.variations[i];
        printf("\e[%sm%c\e[0m(%u) ->", var->color_code, var->symbol, var->base_weight);
        for (unsigned int j = 0; j < var->num_possible; j++)
        {
            struct variation_neighbor *neighbor = &var->possible_neighbors[j];
            printf(" \e[%sm%c\e[0m(%u)", neighbor->neighbor->color_code, neighbor->neighbor->symbol, var->possible_neighbors[j].weight);
        }
        printf("\n");
    }
    printf("\n");

    // create world
    struct world world;
    world_init(&world, &variations, world_height, world_width);

    // hide cursor
    printf("\e[?25l");
    if (interactive_mode)
    {
        printf("world generation step by step:\n");

        // initial world
        world_print(&world);
        world_generate_step(&world);

        useconds_t sleep_usec = 20 * 1000;
        bool changed;
        do
        {
            // move cursor to overwrite last printout
            printf("\033[%dA", world.height); // move up
            printf("\033[%dD", world.width);  // move left
            world_print(&world);
            usleep(sleep_usec);
            changed = world_generate_step(&world);
        } while (changed);
    }
    else
    {
        world_generate(&world);
        printf("world:\n");
        world_print(&world);
    }

    // show cursor again
    printf("\e[?25h");

    // clean up
    world_destroy(&world);

    variation_cleanup(&variations);
}