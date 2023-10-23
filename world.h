#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

#include "tile.h"
#include "tile_variation.h"

struct world
{
    unsigned int height, width;
    struct tile *tiles;
};

void world_init(struct world *world, struct all_variations *variations, unsigned int height, unsigned int width);
void world_destroy(struct world *world);
void world_generate(struct world *world);
bool world_generate_step(struct world *world);
struct tile *world_get_tile(struct world *world, unsigned int x, unsigned int y);

#endif