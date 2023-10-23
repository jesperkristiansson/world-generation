#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

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

    // seed the random number generator
    srand(time(NULL));

    // get variations
    struct all_variations variations = variation_get_all();

    printf("all variations: \n");
    for (unsigned int i = 0; i < variations.num_variations; i++)
    {
        struct variation *var = &variations.variations[i];
        printf("%d ->", var->value);
        for (unsigned int j = 0; j < var->num_possible; j++)
        {
            printf(" %d", var->possible_neighbors[j]->value);
        }
        printf("\n");
    }
    printf("\n");

    // create world
    struct world world;
    world_init(&world, &variations, world_height, world_width);

    // generation loop
    world_generate(&world);

    printf("world:\n");
    for (int r = 0; r < world_height; r++)
    {
        for (int c = 0; c < world_width; c++)
        {
            struct tile *tile = world_get_tile(&world, r, c);
            printf("%d", tile->set_variation->value);
        }
        printf("\n");
    }

    // clean up
    world_destroy(&world);

    variation_cleanup(&variations);
}