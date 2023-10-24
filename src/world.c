
#include "world.h"

#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define WORLD_TILE(world, x, y) (world)->tiles[(y) * (world)->width + (x)]

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

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            struct tile *tile = &WORLD_TILE(world, x, y);
            const unsigned int max_neighbors = 4;
            struct tile **neighbors = malloc(max_neighbors * sizeof(struct tile *)); // should probably be alloced for all tiles at once
            unsigned int idx = 0;
            if (y != 0)
            {
                neighbors[idx++] = &WORLD_TILE(world, x, y - 1);
            }
            if (y != height - 1)
            {
                neighbors[idx++] = &WORLD_TILE(world, x, y + 1);
            }
            if (x != 0)
            {
                neighbors[idx++] = &WORLD_TILE(world, x - 1, y);
            }
            if (x != width - 1)
            {
                neighbors[idx++] = &WORLD_TILE(world, x + 1, y);
            }
            tile_init(tile, variations, neighbors, idx);
            tile_bucket_add(&world->buckets[tile->num_variations - 1], tile);
        }
    }
}

void world_destroy(struct world *world)
{
    for (unsigned int y = 0; y < world->height; y++)
    {
        for (unsigned int x = 0; x < world->width; x++)
        {
            struct tile *tile = &WORLD_TILE(world, x, y);
            tile_teardown(tile);
        }
    }
    free(world->tiles);

    for (unsigned int i = 0; i < world->num_buckets; i++)
    {
        tile_bucket_cleanup(&world->buckets[i]);
    }
    free(world->buckets);
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
        if (candidate != NULL)
        {
            break;
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

void world_get_position(struct world *world, struct tile *tile, unsigned int *x, unsigned int *y)
{
    int tile_index = tile - world->tiles;
    *y = tile_index / world->width;
    *x = tile_index % world->width;
}

void world_print(struct world *world)
{
    for (unsigned int y = 0; y < world->height; y++)
    {
        for (unsigned int x = 0; x < world->width; x++)
        {
            struct tile *tile = &WORLD_TILE(world, x, y);
            if (tile->is_set)
            {
                printf("\e[%sm%c", tile->set_variation->color_code, tile->set_variation->symbol);
            }
            else
            {
                printf("\e[0m%d", tile->num_variations);
            }
        }
        printf("\n");
    }
    // reset colors
    printf("\e[0m");
}