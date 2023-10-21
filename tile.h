#ifndef TILE_H
#define TILE_H

// a tile represents one element of the world grid

#include "tile_variation.h"
#include <stdbool.h>

struct tile
{
    unsigned int num_neighbors;             // number of neighboring tiles
    struct tile **neighbors;                // pointers to the neighboring tilesd
    unsigned int num_variations;            // number of possible variations for this tile, varies during generation
    struct variation **possible_variations; // all variations that the tile can be assigned, varies during generation
    bool is_set;                            // whether the tile has been assigned a variation or not
    struct variation *set_variation;        // only set if is_set == true
};

void tile_init(struct tile *tile, struct all_variations *variations, struct tile **neighbors, unsigned int num_neighbors);
void tile_teardown(struct tile *tile);
void tile_set(struct tile *tile);
#endif