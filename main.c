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

    if (argc >= 3)
    {
        world_width = atoi(argv[1]);
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
    for (int r = 0; r < world_height; r++)
    {
        for (int c = 0; c < world_width; c++)
        {
            struct tile *tile = world_get_tile(&world, r, c);
            // printf("%d", tile->num_variations);
            printf("\e[%sm%c", tile->set_variation->color_code, tile->set_variation->symbol);
        }
        printf("\n");
    }
    // reset colors
    printf("\e[0m");

    // clean up
    world_destroy(&world);

    variation_cleanup(&variations);
}