
#include "tile_bucket.h"

void tile_bucket_init(struct tile_bucket *bucket, unsigned int cap)
{
    bucket->tiles = malloc(cap * sizeof(*bucket->tiles));
    bucket->capacity = cap;
    bucket->write_index = 0;
    bucket->read_index = 0;
}

void tile_bucket_add(struct tile_bucket *bucket, struct tile *tile)
{
    if (bucket->write_index == bucket->capacity)
    {
        bucket->capacity *= 2;
        bucket->tiles = realloc(bucket->tiles, bucket->capacity * sizeof(*bucket->tiles)); // should probably shift elements
    }
    bucket->tiles[bucket->write_index++] = tile;
}

unsigned int tile_bucket_size(struct tile_bucket *bucket)
{
    return bucket->write_index - bucket->read_index;
}

struct tile *tile_bucket_get(struct tile_bucket *bucket)
{
    if (bucket->read_index == bucket->write_index)
    {
        return NULL;
    }

    return bucket->tiles[bucket->read_index++];
}

void tile_bucket_cleanup(struct tile_bucket *bucket)
{
    free(bucket->tiles);
}