#ifndef TILE_BUCKET_H
#define TILE_BUCKET_H

#include "tile.h"

struct tile_bucket
{
    struct tile **tiles;
    unsigned int size;
    unsigned int capacity;
};

void tile_bucket_init(struct tile_bucket *bucket, unsigned int cap);
void tile_bucket_add(struct tile_bucket *bucket, struct tile *tile);
struct tile *tile_bucket_pop(struct tile_bucket *bucket);
void tile_bucket_cleanup(struct tile_bucket *bucket);

#endif