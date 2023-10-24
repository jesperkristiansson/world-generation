
#include "world.h"

#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define WORLD_TILE(world, x, y) (world)->tiles[(x) * (world)->width + (y)]

void world_init(struct world *world, struct all_variations *variations, unsigned int height, unsigned int width)
{
    // seed the random number generator, add the seed as attribute to world instead?
    srand(time(NULL));

    world->height = height;
    world->width = width;

    unsigned int num_tiles = height * width;
    world->tiles = malloc(num_tiles * sizeof(struct tile));

    const unsigned int bucket_starting_cap = (num_tiles + 99) / 100;
    world->num_buckets = variations->num_variations;
    world->buckets = malloc(variations->num_variations * sizeof(struct tile_bucket));
    for (unsigned int i = 0; i < variations->num_variations; i++)
    {
        tile_bucket_init(&world->buckets[i], bucket_starting_cap);
    }

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
            tile_bucket_add(&world->buckets[tile->num_variations - 1], tile);
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
    while (world_generate_step(world))
    { /* do nothing */
    }
}

bool world_generate_step(struct world *world)
{
    struct tile *candidate = NULL;
    for (unsigned int i = 0; i < world->num_buckets; i++)
    {
        struct tile_bucket *bucket = &world->buckets[i];
        while (bucket->size > 0)
        {
            candidate = tile_bucket_pop(bucket);
            if (!candidate->is_set)
            {
                break;
            }
            candidate = NULL;
        }
    }
    if (candidate == NULL)
    {
        return false;
    }

    // update generation
    tile_set(candidate, world->buckets);
    return true;
}

struct tile *world_get_tile(struct world *world, unsigned int x, unsigned int y)
{
    return &WORLD_TILE(world, x, y);
}