#ifndef TILE_BUCKET_H
#define TILE_BUCKET_H

#include "tile.h"

struct tile_bucket
{
    struct tile **tiles;
    unsigned int write_index;
    unsigned int read_index;
    unsigned int capacity;
};

void tile_bucket_init(struct tile_bucket *bucket, unsigned int cap);
void tile_bucket_add(struct tile_bucket *bucket, struct tile *tile);
unsigned int tile_bucket_size(struct tile_bucket *bucket);
struct tile *tile_bucket_get(struct tile_bucket *bucket);
void tile_bucket_clear(struct tile_bucket *bucket);
void tile_bucket_cleanup(struct tile_bucket *bucket);

#endif