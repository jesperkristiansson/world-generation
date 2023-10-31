#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

#include "tile.h"
#include "tile_bucket.h"
#include "tile_variation.h"

struct world
{
    struct tile *tiles;
    struct tile_bucket *buckets;
    unsigned int *last_val; // keeps track of which tiles have changed, used to optimized which tiles are printed
    unsigned int height, width;
    unsigned int num_buckets;
};

void world_init(struct world *world, unsigned int  seed, struct all_variations *variations, unsigned int height, unsigned int width);
void world_destroy(struct world *world);
void world_generate(struct world *world);
bool world_generate_step(struct world *world);
struct tile *world_get_tile(struct world *world, unsigned int x, unsigned int y);
void world_get_position(struct world *world, struct tile *tile, unsigned int *x, unsigned int *y);
void world_print(struct world *world);
void world_print_update(struct world *world); // world_print must have been called before and no additional print outs

#endif