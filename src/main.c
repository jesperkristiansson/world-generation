#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "world.h"
#include "tile.h"
#include "tile_variation.h"

int main(int argc, char **argv)
{
    int world_width = 100;
    int world_height = 25;

    if (argc >= 2)
    {
        world_width = atoi(argv[1]);
    }
    if (argc >= 3)
    {
        world_height = atoi(argv[2]);
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
    world_generate(&world);

    // for (unsigned int i = 0; i < 10; ++i)
    // {
    //     if (!world_generate_step(&world))
    //     {
    //         break;
    //     }
    // }

    printf("world:\n");
    world_print(&world);

    // clean up
    world_destroy(&world);

    variation_cleanup(&variations);
}