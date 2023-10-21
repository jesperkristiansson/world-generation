#include <stdlib.h>
#include <assert.h>

#include "tile.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void tile_init(struct tile *tile, int x, int y, struct all_variations *variations, struct tile **neighbors, unsigned int num_neighbors)
{
    tile->x = x;
    tile->y = y;
    tile->num_neighbors = num_neighbors;
    tile->neighbors = neighbors;
    tile->is_set = false;
    tile->set_variation = NULL;

    tile->num_variations = variations->num_variations;
    tile->possible_variations = malloc(tile->num_variations * sizeof(struct variation *));
    for (unsigned int i = 0; i < variations->num_variations; i++)
    {
        tile->possible_variations[i] = &variations->variations[i];
    }
}

static void tile_update(struct tile *tile, struct variation **allowed_variations, unsigned int num_allowed_variations)
{
    if (tile->is_set)
    {
        return;
    }

    unsigned int upper_bound = MIN(tile->num_variations, num_allowed_variations);
    struct variation **new_variations = malloc(upper_bound * sizeof(struct variation *));
    unsigned int idx = 0;
    for (unsigned int i = 0; i < num_allowed_variations; i++)
    {
        struct variation *var = allowed_variations[i];
        for (unsigned int j = 0; j < tile->num_variations; j++)
        {
            struct variation *var2 = tile->possible_variations[j];
            if (var->value == var2->value)
            {
                new_variations[idx++] = var;
                break;
            }
        }
    }

    if (idx < tile->num_variations)
    {
        free(tile->possible_variations);
        tile->num_variations = idx;
        tile->possible_variations = new_variations;
        // update neighbors
    }
    else
    {
        free(new_variations);
    }
}

void tile_set(struct tile *tile)
{
    assert(!tile->is_set);
    assert(tile->num_variations > 0);

    // set variation for tile
    int rand_i = rand() % tile->num_variations;
    struct variation *var = tile->possible_variations[rand_i];
    tile->set_variation = var;
    tile->is_set = true;
    tile->num_variations = 0;

    // update neighbors
    for (unsigned int i = 0; i < tile->num_neighbors; i++)
    {
        struct tile *neighbor = tile->neighbors[i];
        tile_update(neighbor, var->possible_neighbors, var->num_possible);
    }
}