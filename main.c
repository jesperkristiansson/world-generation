#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#include "tile.h"
#include "tile_variation.h"

int main(int argc, char **argv)
{
    int world_width = 100;
    int world_height = 25;

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

    // create tiles
    struct tile world[world_height][world_width];
    for (int r = 0; r < world_height; r++)
    {
        for (int c = 0; c < world_width; c++)
        {
            struct tile *tile = &world[r][c];
            const unsigned int max_neighbors = 4;
            struct tile **neighbors = malloc(max_neighbors * sizeof(struct tile *)); // should probably be alloced for all tiles at once
            unsigned int idx = 0;
            if (r != 0)
            {
                neighbors[idx++] = &world[r - 1][c];
            }
            if (r != world_height - 1)
            {
                neighbors[idx++] = &world[r + 1][c];
            }
            if (c != 0)
            {
                neighbors[idx++] = &world[r][c - 1];
            }
            if (c != world_width - 1)
            {
                neighbors[idx++] = &world[r][c + 1];
            }
            tile_init(tile, c, r, &variations, neighbors, idx);
        }
    }

    // generation loop
    while (1)
    {
        struct tile *candidate = NULL;
        unsigned int lowest_num_variations = UINT_MAX;
        for (int r = 0; r < world_height; r++) // use a proper data structure instead, like a binary heap
        {
            for (int c = 0; c < world_width; c++)
            {
                struct tile *tile = &world[r][c];
                if (!tile->is_set && tile->num_variations < lowest_num_variations)
                {
                    lowest_num_variations = tile->num_variations;
                    candidate = tile;
                }
            }
        }
        if (candidate == NULL)
        {
            break;
        }

        // update generation
        tile_set(candidate);
    }

    printf("world:\n");
    for (int r = 0; r < world_height; r++)
    {
        for (int c = 0; c < world_width; c++)
        {
            struct tile *tile = &world[r][c];
            printf("%d", tile->set_variation->value);
        }
        printf("\n");
    }

    // clean up
    for (int r = 0; r < world_height; r++)
    {
        for (int c = 0; c < world_width; c++)
        {
            struct tile *tile = &world[r][c];
            tile_teardown(tile);
        }
    }

    variation_cleanup(&variations);
}