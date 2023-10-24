
#include "tile_bucket.h"

void tile_bucket_init(struct tile_bucket *bucket, unsigned int cap)
{
    bucket->tiles = malloc(cap * sizeof(*bucket->tiles));
    bucket->capacity = cap;
    bucket->size = 0;
}

void tile_bucket_add(struct tile_bucket *bucket, struct tile *tile)
{
    if (bucket->size >= bucket->capacity)
    {
        bucket->capacity *= 2;
        bucket->tiles = realloc(bucket->tiles, bucket->capacity * sizeof(*bucket->tiles));
    }
    bucket->tiles[bucket->size++] = tile;
}

struct tile *tile_bucket_pop(struct tile_bucket *bucket)
{
    if (bucket->size == 0)
    {
        return NULL;
    }

    return bucket->tiles[--bucket->size];
}

void tile_bucket_cleanup(struct tile_bucket *bucket)
{
    free(bucket->tiles);
}