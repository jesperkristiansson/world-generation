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

    if (interactive_mode)
    {
        printf("world generation step by step:\n");
        bool changed;
        do
        {
            sleep(1);
            changed = world_generate_step(&world);
            world_print(&world);
            printf("\n");
        } while (changed);
    }
    else
    {
        world_generate(&world);
        printf("world:\n");
        world_print(&world);
    }

    // clean up
    world_destroy(&world);

    variation_cleanup(&variations);
}