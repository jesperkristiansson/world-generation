
#include "world.h"

#include <limits.h>

#define WORLD_TILE(world, x, y) (world)->tiles[(x) * (world)->width + (y)]

void world_init(struct world *world, struct all_variations *variations, unsigned int height, unsigned int width)
{
    world->height = height;
    world->width = width;

    unsigned int num_tiles = height * width;
    world->tiles = malloc(num_tiles * sizeof(struct tile));
    for (unsigned int r = 0; r < height; r++)
    {
        for (unsigned int c = 0; c < width; c++)
        {
            struct tile *tile = &WORLD_TILE(world, r, c);
            const unsigned int max_neighbors = 4;
            struct tile **neighbors = malloc(max_neighbors * sizeof(struct tile *)); // should probably be alloced for all tiles at once
            unsigned int idx = 0;
            if (r != 0)
            {
                neighbors[idx++] = &WORLD_TILE(world, r - 1, c);
            }
            if (r != height - 1)
            {
                neighbors[idx++] = &WORLD_TILE(world, r + 1, c);
            }
            if (c != 0)
            {
                neighbors[idx++] = &WORLD_TILE(world, r, c - 1);
            }
            if (c != width - 1)
            {
                neighbors[idx++] = &WORLD_TILE(world, r, c + 1);
            }
            tile_init(tile, variations, neighbors, idx);
        }
    }
}

void world_destroy(struct world *world)
{
    for (unsigned int r = 0; r < world->height; r++)
    {
        for (unsigned int c = 0; c < world->width; c++)
        {
            struct tile *tile = &WORLD_TILE(world, r, c);
            tile_teardown(tile);
        }
    }
    free(world->tiles);
}

void world_generate(struct world *world)
{
    while (1)
    {
        struct tile *candidate = NULL;
        unsigned int lowest_num_variations = UINT_MAX;
        for (unsigned int r = 0; r < world->height; r++) // use a proper data structure instead, like a binary heap
        {
            for (unsigned int c = 0; c < world->width; c++)
            {
                struct tile *tile = &WORLD_TILE(world, r, c);
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
}

struct tile *world_get_tile(struct world *world, unsigned int x, unsigned int y)
{
    return &WORLD_TILE(world, x, y);
}