
#include "world.h"

#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define WORLD_TILE(world, x, y) (world)->tiles[(y) * (world)->width + (x)]
#define WORLD_TILE_INDEX(world, x, y) ((y) * (world)->width + (x))

void world_init(struct world *world, unsigned int seed, struct all_variations *variations, unsigned int height, unsigned int width)
{
    // seed the random number generator, add the seed as attribute to world instead?
    srand(seed);

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

    tile_bucket_init(&world->changed_list, bucket_starting_cap);

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
            tile_init(tile, x, y, variations, neighbors, idx);
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

    tile_bucket_cleanup(&world->changed_list);
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
        while (tile_bucket_size(bucket) > 0)
        {
            candidate = tile_bucket_get(bucket);
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
    tile_set(candidate, world->buckets, &world->changed_list);
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

void world_print_update(struct world *world)
{
    // hide cursor
    printf("\e[?25l");

    // move cursor to start of world
    printf("\033[%dA", world->height); // move up
    // printf("\033[%dD", world->width);  // move left
    unsigned int cursor_x = 0;
    unsigned int cursor_y = 0;

    char *last_color = "0";
    while (tile_bucket_size(&world->changed_list) > 0)
    {
        struct tile *tile = tile_bucket_get(&world->changed_list);
        unsigned int x = tile->x;
        unsigned int y = tile->y;

        int delta_x = (int)x - (int)cursor_x;
        int delta_y = (int)y - (int)cursor_y;

        // move cursor
        if (delta_x < 0)
        {
            printf("\033[%dD", -delta_x); // move left
        }
        else if (delta_x > 0)
        {
            printf("\033[%dC", delta_x); // move right
        }

        if (delta_y < 0)
        {
            printf("\033[%dA", -delta_y); // move up
        }
        else if (delta_y > 0)
        {
            printf("\033[%dB", delta_y); // move down
        }

        if (tile->is_set)
        {
            char *new_color = tile->set_variation->color_code;
            if (strcmp(new_color, last_color) != 0)
            {
                last_color = new_color;
                printf("\e[%sm", new_color);
            }
            printf("%c", tile->set_variation->symbol);
        }
        else
        {
            char *new_color = "0";
            if (strcmp(new_color, last_color) != 0)
            {
                last_color = new_color;
                printf("\e[0m");
            }
            printf("%d", tile->num_variations);
        }
        cursor_x += delta_x + 1;
        cursor_y += delta_y;
    }
    // move cursor to start of line after world printout
    printf("\033[%dD", cursor_x);
    printf("\033[%dB", (int)world->height - (int)cursor_y);

    // reset colors
    printf("\e[0m");

    // show cursor again
    printf("\e[?25h");

    fflush(stdout);
}

void world_print(struct world *world)
{
    char *last_color = "0";
    for (unsigned int y = 0; y < world->height; y++)
    {
        for (unsigned int x = 0; x < world->width; x++)
        {
            struct tile *tile = &WORLD_TILE(world, x, y);
            if (tile->is_set)
            {
                char *new_color = tile->set_variation->color_code;
                if (strcmp(new_color, last_color) != 0)
                {
                    last_color = new_color;
                    printf("\e[%sm", new_color);
                }
                printf("%c", tile->set_variation->symbol);
            }
            else
            {
                char *new_color = "0";
                if (strcmp(new_color, last_color) != 0)
                {
                    last_color = new_color;
                    printf("\e[0m");
                }
                printf("%d", tile->num_variations);
            }
        }
        printf("\n");
    }
    // reset colors
    printf("\e[0m");

    tile_bucket_clear(&world->changed_list);
}